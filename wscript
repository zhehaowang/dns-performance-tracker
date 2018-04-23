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

    opt.add_option('--mysql-include',
                   action = 'store',
                   default = '/usr/local/include/mysql/',
                   dest = 'mysql_inc',
                   help = '''MySQL library include path''')

    opt.add_option('--with-tests',
                   action = 'store_true',
                   default = False,
                   dest = 'with_tests',
                   help = '''Compile unit tests''')

def configure(conf):
    conf.load('compiler_c compiler_cxx')

    # openssl header is needed but not library
    #conf.check_cxx(lib = 'ssl', define_name = 'HAVE_SSL', use = 'OPENSSL', uselib_store = 'openssl', mandatory = True)
    conf.env.INCLUDES_EXTRA = [conf.options.openssl_inc, conf.options.mysql_inc]
    conf.env.append_value('INCLUDES', conf.env.INCLUDES_EXTRA)
    conf.check_cxx(header_name = 'openssl/ssl.h')
    conf.check_cxx(lib = 'ldns', define_name = 'HAVE_LDNS', uselib_store = 'ldns', mandatory = True)

    conf.check_cxx(header_name = 'mysql_version.h')
    conf.check_cxx(lib = 'mysqlclient', define_name = 'HAVE_MYSQLCLIENT', uselib_store = 'mysqlclient', mandatory = True)
    conf.check_cxx(lib = 'mysqlpp', define_name = 'HAVE_MYSQLPP', uselib_store = 'mysqlpp', mandatory = True)

    if conf.options.with_tests:
        conf.check_cxx(lib = 'gtest', define_name = 'HAVE_GTEST', uselib_store = 'gtest', mandatory = True)

    conf.env['WITH_TESTS'] = conf.options.with_tests
    conf.env.append_value('CXXFLAGS', '-std=c++11');

def build(bld):
    bld(features='subst', source='conf/db.sample.conf', target='db.conf', is_copy=True)
    bld(features='subst', source='conf/sites.sample.conf', target='sites.conf', is_copy=True)
    bld(
        features = 'c cxx cxxprogram',
        source   = bld.path.ant_glob('src/**/*.cpp', excl = ['src/**/*.t.cpp']),
        use      = 'ldns mysqlclient mysqlpp gtest',
        target   = 'dns_performance_tracker',
        export_includes = '.',
        includes = 'src src/query src/report thirdparty/loguru'
    )

    if bld.env['WITH_TESTS']:
        bld(
            features = 'c cxx cxxprogram',
            source   = bld.path.ant_glob('src/**/*.t.cpp', excl = ['src/**/*.m.cpp']),
            use      = 'gtest',
            target   = 'tests',
            export_includes = '.',
            includes = 'src src/query src/report thirdparty/loguru'
        )
        
