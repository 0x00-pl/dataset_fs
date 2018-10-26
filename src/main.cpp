#include <boost/python/numpy.hpp>
#include <boost/function.hpp>
#include <boost/python.hpp>
#include <Python.h>
#include <stdio.h>
#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;
using namespace boost::python;
using namespace boost::python::numpy;
namespace py = boost::python;
namespace np = boost::python::numpy;

#define no_thread_safe(x) x##_unlocked

ndarray return_100_zeros()
{
    py::tuple shape = py::make_tuple ( 100 );
    np::dtype dtype = dtype::get_builtin<float>();
    ndarray a = np::zeros ( shape, dtype );
    return a;
}

void test_argument ( FILE* f )
{
    fprintf ( f, "Written from C++\n" );
}

class fwarp
{
public:
    fwarp ( const char* filename )
    {
        f = open ( filename, O_RDONLY );
    }
    ~fwarp() {
        close(f);
    }

    template<typename T>
    ndarray read ( int64_t start, int64_t length )
    {
        lseek64( f, start, SEEK_SET );
        T buff[length];
        if ( ::read(f, buff, length*sizeof ( T )) <= length*sizeof ( T )) {
            return np::zeros ( py::make_tuple ( 0 ), dtype::get_builtin<T>() );
        }
        py::object own;
        auto ret = np::from_data (
                       buff,
                       dtype::get_builtin<T>(),
                       py::make_tuple ( length ),
                       py::make_tuple ( sizeof ( T ) ),
                       own
                   );
        return ret;
    }
//     ndarray read_int8 ( int64_t start, int64_t length )
//     {
//         fseeko64 ( f, start, SEEK_SET );
//         auto buff = np::empty ( py::make_tuple ( length ), dtype::get_builtin<int8_t>() );
//         if ( no_thread_safe ( fread ) ( buff.get_data(), length, 1, f ) != 1 ) {
//             return np::zeros ( py::make_tuple ( 0 ), dtype::get_builtin<int8_t>() );
//         }
// //         py::object own;
// //         auto ret = np::from_data (
// //                        buff,
// //                        dtype::get_builtin<int8_t>(),
// //                        py::make_tuple ( length ),
// //                        py::make_tuple ( sizeof ( int8_t ) ),
// //                        own
// //                    );
//         return buff;
//     }

    int f;
};

BOOST_PYTHON_MODULE ( libcpython_test_lib )
{
    Py_Initialize();
    np::initialize();
    def ( "return_100_zeros", return_100_zeros );
    def ( "test_argument", test_argument );

    py::class_<fwarp> ( "fwarp", init<const char*>() )
    .def ( "read_int8", &fwarp::read<int8_t> )
    .def ( "read_int16", &fwarp::read<int16_t> );
}

int main ( int argc, char **argv )
{
    Py_Initialize();
    np::initialize();
    cout<<"hello world"<<endl;
    fwarp fw ( "/data/rand_file" );
    ndarray file_part = fw.read<int8_t> ( 0, 1000 );
    cout<<extract<char const *> ( py::str ( file_part ) ) <<endl;
}
