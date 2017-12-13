# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

# def options(opt):
#     pass

# def configure(conf):
#     conf.check_nonfatal(header_name='stdint.h', define_name='HAVE_STDINT_H')

def build(bld):
    module = bld.create_ns3_module('dtm', ['core'])
    module.source = [
#        'model/dtm.cc',
        'model/dtm-ptable.cc',
        'model/dtm-packet.cc',
        'model/dtm-wqueue.cc',
#        'helper/dtm-helper.cc'
        ]

    module_test = bld.create_ns3_module_test_library('dtm')
    module_test.source = [
        'test/dtm-test-suite.cc',
        ]

    headers = bld(features='ns3header')
    headers.module = 'dtm'
    headers.source = [
#        'model/dtm.h',
        'model/dtm-ptable.h',
        'model/dtm-packet.h',
        'model/dtm-wqueue.h',
#        'helper/dtm-helper.h',
        ]

    #if bld.env.ENABLE_EXAMPLES:
    #    bld.recurse('examples')

    # bld.ns3_python_bindings()

