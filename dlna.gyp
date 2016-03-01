#
# Copyright (c) 2014 UC. All rights reserved.
#
# author: luozy3@ucweb.com
# create time: 2015-03-20
# history:
#   2015-03-20 init by luozy3
#
# create ninja file:
#   build/gyp_chromium -DOS=android --depth=. --toplevel-dir=`pwd` uc/dlna/dlna.gyp
#
# build:
#   ninja -C out/Release ucdlna -j7
#
{
  'targets': [
    {
      'target_name': 'ucdlna',
      # executable shared_library static_library
      'type': 'executable',
      #'android_unmangled_name': 1,
      'dependencies': [
        '../../third_party/expat/expat.gyp:expat'
      ],
      'variables': {
        'clinkc_include_home': 'clinkc/include',
        'clinkc_src_home': 'clinkc/src/cybergarage',
        'src_dirs': [
          'clinkc_include_home',
          '<(clinkc_src_home)',
          'src',
        ],
      },
      'sources': [
        '<(clinkc_src_home)/util/cmutex.c',
        '<(clinkc_src_home)/util/cthread_list.c',
        '<(clinkc_src_home)/util/cdictionary_elem.c',
        '<(clinkc_src_home)/util/clog.c',
        '<(clinkc_src_home)/util/cstring.c',
        '<(clinkc_src_home)/util/cstring_function.c',
        '<(clinkc_src_home)/util/ctime.c',
        '<(clinkc_src_home)/util/cstring_tokenizer.c',
        '<(clinkc_src_home)/util/ctime_wince.c',
        '<(clinkc_src_home)/util/cthread.c',
        '<(clinkc_src_home)/util/ccond.c',
        '<(clinkc_src_home)/util/cdictionary.c',
        '<(clinkc_src_home)/util/clist.c',
        '<(clinkc_src_home)/http/chttp_header.c',
        '<(clinkc_src_home)/http/chttp_request.c',
        '<(clinkc_src_home)/http/chttp_header_list.c',
        '<(clinkc_src_home)/http/chttp_packet.c',
        '<(clinkc_src_home)/http/chttp.c',
        '<(clinkc_src_home)/http/chttp_response.c',
        '<(clinkc_src_home)/http/chttp_date.c',
        '<(clinkc_src_home)/http/chttp_server.c',
        '<(clinkc_src_home)/http/chttp_persistent_connection.c',
        '<(clinkc_src_home)/http/chttp_server_list.c',
        '<(clinkc_src_home)/xml/cxml_function.c',
        '<(clinkc_src_home)/xml/cxml_node.c',
        '<(clinkc_src_home)/xml/cxml_node_list.c',
        '<(clinkc_src_home)/xml/cxml_attribute.c',
        '<(clinkc_src_home)/xml/cxml_parser_libxml2.c',
        '<(clinkc_src_home)/xml/cxml_parser.c',
        '<(clinkc_src_home)/xml/cxml_attribute_list.c',
        '<(clinkc_src_home)/xml/cxml_parser_expat.c',
        '<(clinkc_src_home)/io/cfile_list.c',
        '<(clinkc_src_home)/io/cfile.c',
        '<(clinkc_src_home)/soap/csoap_request.c',
        '<(clinkc_src_home)/soap/csoap.c',
        '<(clinkc_src_home)/soap/csoap_response.c',
        '<(clinkc_src_home)/net/cinterface_function.c',
        '<(clinkc_src_home)/net/curi.c',
        '<(clinkc_src_home)/net/cinterface_list.c',
        '<(clinkc_src_home)/net/csocket.c',
        '<(clinkc_src_home)/net/cdatagram_packet.c',
        '<(clinkc_src_home)/net/curl.c',
        '<(clinkc_src_home)/net/csocket_list.c',
        '<(clinkc_src_home)/net/cnet_function.c',
        '<(clinkc_src_home)/net/cinterface.c',
        '<(clinkc_src_home)/upnp/cupnp_status.c',
        '<(clinkc_src_home)/upnp/cupnp_function.c',
        '<(clinkc_src_home)/upnp/cdevice_http_server.c',
        '<(clinkc_src_home)/upnp/ccontrolpoint_event.c',
        '<(clinkc_src_home)/upnp/cdevice.c',
        '<(clinkc_src_home)/upnp/ccontrolpoint.c',
        '<(clinkc_src_home)/upnp/cservice_ssdp_server.c',
        '<(clinkc_src_home)/upnp/cstatevariable.c',
        '<(clinkc_src_home)/upnp/cservice_std_dcp.c',
        '<(clinkc_src_home)/upnp/cservice_notify.c',
        '<(clinkc_src_home)/upnp/cargument_list.c',
        '<(clinkc_src_home)/upnp/control/ccontrol_function.c',
        '<(clinkc_src_home)/upnp/control/cquery_response.c',
        '<(clinkc_src_home)/upnp/control/cquery_ctrl.c',
        '<(clinkc_src_home)/upnp/control/caction_request.c',
        '<(clinkc_src_home)/upnp/control/caction_ctrl.c',
        '<(clinkc_src_home)/upnp/control/caction_response.c',
        '<(clinkc_src_home)/upnp/control/cquery_request.c',
        '<(clinkc_src_home)/upnp/ccontrolpoint_http_server.c',
        '<(clinkc_src_home)/upnp/cicon_list.c',
        '<(clinkc_src_home)/upnp/event/csubscription.c',
        '<(clinkc_src_home)/upnp/event/csubscription_request.c',
        '<(clinkc_src_home)/upnp/event/cnotify_request.c',
        '<(clinkc_src_home)/upnp/event/ceventlistener_list.c',
        '<(clinkc_src_home)/upnp/event/csubscription_response.c',
        '<(clinkc_src_home)/upnp/event/cproperty.c',
        '<(clinkc_src_home)/upnp/event/csubscriber.c',
        '<(clinkc_src_home)/upnp/event/csubscriber_list.c',
        '<(clinkc_src_home)/upnp/event/cproperty_list.c',
        '<(clinkc_src_home)/upnp/ssdp/cssdp_server.c',
        '<(clinkc_src_home)/upnp/ssdp/cssdp_request.c',
        '<(clinkc_src_home)/upnp/ssdp/cssdp_socket.c',
        '<(clinkc_src_home)/upnp/ssdp/cssdp_response_server.c',
        '<(clinkc_src_home)/upnp/ssdp/cssdp_response_server_list.c',
        '<(clinkc_src_home)/upnp/ssdp/cssdp.c',
        '<(clinkc_src_home)/upnp/ssdp/cssdp_response.c',
        '<(clinkc_src_home)/upnp/ssdp/cssdp_packet.c',
        '<(clinkc_src_home)/upnp/ssdp/cssdp_server_list.c',
        '<(clinkc_src_home)/upnp/ssdp/chttpu_socket.c',
        '<(clinkc_src_home)/upnp/ssdp/chttpmu_socket.c',
        '<(clinkc_src_home)/upnp/cdevice_list.c',
        '<(clinkc_src_home)/upnp/cservice_list.c',
        '<(clinkc_src_home)/upnp/cargument.c',
        '<(clinkc_src_home)/upnp/cicon.c',
        '<(clinkc_src_home)/upnp/caction_list.c',
        '<(clinkc_src_home)/upnp/cservicestate_table.c',
        '<(clinkc_src_home)/upnp/cdevice_ssdp_server.c',
        '<(clinkc_src_home)/upnp/cdevice_advertiser.c',
        '<(clinkc_src_home)/upnp/caction.c',
        '<(clinkc_src_home)/upnp/cservice.c',

        'src/ucdlna_control.c',
        'src/ucdlna_log.c',

        'test/linux/ucdlna_control_test.c'
      ],
      'include_dirs': [
        '<(clinkc_include_home)',
        'include',
      ],
      'defines': [
        'PIC',
        'HAVE_CONFIG_H',
      ],
      'cflags': [
        '-fPIC',
        '-fvisibility=hidden',
        '-ffunction-sections',
        '-fdata-sections',
      ],
      'link_settings': {
        'ldflags': [
          '-Wl,--gc-sections',
        ],
      },
      'conditions': [
        ['OS=="android"', {
          'link_settings': {
            'libraries': [
              '-llog',
            ],
          },
        }],
      ],

    },
  ]
}
