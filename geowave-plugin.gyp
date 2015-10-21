{
   # These are just the defaults.  Feel free to override with whatever you require.
   # Any variables ending with '%' can be overridden (e.g. -D java_home='abc').
   'variables': {

      # Because these variables are referenced below, and because they can be
      # overridden, they need to be defined here so that their values resolve
      # first.  
      'variables': {
         'boost_home%': '/usr/local',
         'geowave_home%': '/usr/local',
         'java_home%': '<!(echo $JAVA_HOME)',
         'mapnik_config%': 'mapnik-config',
      },

      # boost
      'boost_includes%': '<(boost_home)/include',
      'boost_libs%': '<(boost_home)/lib',

      # geowave
      'geowave_includes%': '<(geowave_home)/include',
      'geowave_libs%': '<(geowave_home)/lib',

      # java
      # For whatever reason, java_home is not in scope for the conditions
      # section below.  In order for java_home to resolvei there, we need to
      # redefine it here.  This is not an issue when overriding java_home.
      'java_home%': '<!(echo $JAVA_HOME)',
      'java_include_path%': '<(java_home)/include',
      'java_jvm_lib%': '<(java_home)/jre/lib/amd64/server',

      # mapnik
      'mapnik_includes%': '<!(<(mapnik_config) --includes | sed -e "s/-I//g")',
      'mapnik_libs%': '<!(<(mapnik_config) --libs | sed -e "s/-L//g;s/\s.*$//g")',
      'mapnik_name%': '<!(<(mapnik_config) --libs | sed -e "s/.*\s//g;s/-l//g")',
   },
   "conditions": [
      ["OS=='win'", {
         'variables': {
            'java_include_path2%': '<(java_home)/include/win32'
         }
      },
      {
         'variables': {
            'java_include_path2%': '<(java_home)/include/linux'
         }
      }]
   ],
   'includes': [
      'common.gypi'
   ],
   'targets': [
      {
         'target_name': 'geowave-plugin',
         'type': 'shared_library',
         'include_dirs': [
            'include',
            '<@(mapnik_includes)',
            '<(java_include_path)',
            '<(java_include_path2)',
            '<(geowave_includes)',
            '<(boost_includes)'
         ], 
         'sources': [
            'src/geowave_featureset.cpp',
            'src/geowave_datasource.cpp'
         ],
         'link_settings': {
            'libraries': [
               '-l<(mapnik_name)',
               '-ljace',
               '-ljvm',
               '-lboost_thread',
               '-lboost_system'
            ],
            'library_dirs': [
               '<(mapnik_libs)',
               '<(geowave_libs)',
               '<(java_jvm_lib)',
               '<(boost_libs)'
            ]
         }
      }
   ]
}
