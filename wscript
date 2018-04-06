#! /usr/bin/env python
# encoding: utf-8

def options(opt):
    opt.load('compiler_c compiler_cxx')
    opt = opt.add_option_group('Options')

    opt.add_option('--openssl-include',
                   action = 'store',
                   default = '/usr/local/opt/openssl/include',
                   dest = 'openssl_inc',
                   help = '''Openssl library include path''')

def configure(conf):
    print("configure!")

    conf.load('compiler_c compiler_cxx')

    # Default brew installed openssl path on OSX 10.13
    #conf.env.LIBPATH_OPENSSL  = ['/usr/local/opt/openssl/lib']

    conf.env.INCLUDES_OPENSSL = [conf.options.openssl_inc]
    # openssl header is needed but not library
    #conf.check_cxx(lib = 'ssl', define_name = 'HAVE_SSL', use = 'OPENSSL', uselib_store = 'openssl', mandatory = True)
    conf.env.append_value('INCLUDES', conf.env.INCLUDES_OPENSSL)
    conf.check_cxx(msg= 'Checking for header openssl/ssl.h', define_name='HAVE_OPENSSL_HEADER', mandatory = True,
                   fragment='''
                       #include <openssl/ssl.h>
                       int main() { return 0; }
                       ''')

    conf.check_cxx(lib = 'ldns', define_name = 'HAVE_LDNS', uselib_store = 'ldns', mandatory = True)
    conf.check_cxx(lib = 'mysqlpp', define_name = 'HAVE_MYSQLPP', uselib_store = 'mysqlpp', mandatory = True)

def build(bld):
    print("build!")
    bld(
        features = 'c cxx cxxprogram',
        source   = bld.path.ant_glob('src/**/*.cpp'),
        use      = 'ldns',
        target   = 'dns_performance_tracker',
        export_includes = '.',
        includes = 'src src/query src/report'
    )
