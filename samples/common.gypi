{
  'target_defaults': {
    # TODO(bradnelson): This should really be able to be either:
    #   CHROMIUM_BUILD or GOOGLE_CHROME_BUILD
    'defines': ['CHROMIUM_BUILD'],
    'configurations': [
      {
        'configuration_name': 'Debug',
        'conditions': [
          [ 'OS=="mac"', {
            'xcode_settings': {
              'COPY_PHASE_STRIP': 'NO',
              'GCC_OPTIMIZATION_LEVEL': '0',
            }
          }],
          [ 'OS=="win"', {
            'configuration_platform': 'Win32',
            'msvs_configuration_attributes': {
              'OutputDirectory': '$(SolutionDir)$(ConfigurationName)',
              'IntermediateDirectory':
                '$(SolutionDir)$(ConfigurationName)\\obj\\$(ProjectName)',
              'CharacterSet': '1',
            },
            'msvs_settings': {
              'VCCLCompilerTool': {
                'Optimization': '0',
                'PreprocessorDefinitions': ['_DEBUG'],
                'BasicRuntimeChecks': '3',
                'RuntimeLibrary': '1',
              },
              'VCLinkerTool': {
                'LinkIncremental': '2',
              },
              'VCResourceCompilerTool': {
                'PreprocessorDefinitions': ['_DEBUG'],
              },
            },
          }],
        ],
      },
      {
        'configuration_name': 'Release',
        'defines': [
          'NDEBUG',
        ],
        'conditions': [
          [ 'OS=="mac"', {
            'xcode_settings': {
              'DEAD_CODE_STRIPPING': 'YES',
            }
          }],
          [ 'OS=="win"', {
            'configuration_platform': 'Win32',
            'msvs_props': ['release.vsprops'],
          }],
        ],
      },
    ],
  },
  'conditions': [
    [ 'OS=="mac"', {
      'target_defaults': {
        'xcode_settings': {
          'ALWAYS_SEARCH_USER_PATHS': 'NO',
          'GCC_C_LANGUAGE_STANDARD': 'c99',
          'GCC_CW_ASM_SYNTAX': 'NO',
          'GCC_DYNAMIC_NO_PIC': 'YES',
          'GCC_ENABLE_PASCAL_STRINGS': 'NO',
          'GCC_INLINES_ARE_PRIVATE_EXTERN': 'YES',
          'GCC_PRECOMPILE_PREFIX_HEADER': 'YES',
          'GCC_SYMBOLS_PRIVATE_EXTERN': 'YES',
          'GCC_TREAT_WARNINGS_AS_ERRORS': 'YES',
          'GCC_VERSION': '4.2',
          'GCC_WARN_ABOUT_MISSING_NEWLINE': 'YES',
          'MACOSX_DEPLOYMENT_TARGET': '10.5',
          'PREBINDING': 'NO',
          'SDKROOT': 'macosx10.5',
          # xcodebuild_gyp is a temporary name to avoid colliding with the
          # xcodebuild directory used by the non-gyp Xcode build system.
          'SYMROOT': '<(depth)/xcodebuild_gyp',
          'USE_HEADERMAP': 'NO',
          'WARNING_CFLAGS': ['-Wall', '-Wendif-labels'],
        },
        'target_conditions': [
          ['_type=="shared_library"', {
            'xcode_settings': {'GCC_DYNAMIC_NO_PIC': 'NO'},
          }],
        ],
      },
    }],
    [ 'OS=="win"', {
      'variables': {
        'SDK': '$(SolutionDir)../third_party/platformsdk_win2008_6_1/files',
        'SDKIncludes':
          '$(SolutionDir)../third_party/platformsdk_win2008_6_1/files/Include;$(VSInstallDir)/VC/atlmfc/include',
        'SDKLibs':
          '$(SolutionDir)../third_party/platformsdk_win2008_6_1/files/Lib',
      },
      'target_defaults': {
        'defines': [
          '_WIN32_WINNT=0x0600',
          'WINVER=0x0600',
          'WIN32',
          '_WINDOWS',
          '_HAS_EXCEPTIONS=0',
          'NOMINMAX',
          '_CRT_RAND_S',
          'CERT_CHAIN_PARA_HAS_EXTRA_FIELDS',
          'WIN32_LEAN_AND_MEAN',
          '_SECURE_ATL',
          '_HAS_TR1=0',
        ],
        'include_dirs': ['..', '<(SDKIncludes)'],
        'msvs_disable_warnings': [4503, 4819],
        'msvs_settings': {
          'VCCLCompilerTool': {
            'MinimalRebuild': 'false',
            'ExceptionHandling': '0',
            'BufferSecurityCheck': 'true',
            'EnableFunctionLevelLinking': 'true',
            'RuntimeTypeInfo': 'false',
            'WarningLevel': '3',
            'WarnAsError': 'true',
            'DebugInformationFormat': '3',
          },
          'VCLibrarianTool': {
            'AdditionalOptions': '/ignore:4221',
            'OutputFile': '$(OutDir)\\lib\\$(ProjectName).lib',
            'AdditionalLibraryDirectories': '<(SDKLibs)',
          },
          'VCLinkerTool': {
            'AdditionalOptions':
              '/safeseh /dynamicbase /ignore:4199 /ignore:4221 /nxcompat',
            'AdditionalDependencies': 'wininet.lib version.lib msimg32.lib ws2_32.lib usp10.lib psapi.lib',
            'AdditionalLibraryDirectories': '<(SDKLibs)',
            'DelayLoadDLLs': 'dwmapi.dll,uxtheme.dll',
            'GenerateDebugInformation': 'true',
            'MapFileName': '$(OutDir)\\$(TargetName).map',
            'ImportLibrary': '$(OutDir)\\lib\\$(TargetName).lib',
            'TargetMachine': '1',
            'FixedBaseAddress': '1',
          },
          'VCMIDLTool': {
            'GenerateStublessProxies': 'true',
            'TypeLibraryName': '$(InputName).tlb',
            'OutputDirectory': '$(IntDir)',
            'HeaderFileName': '$(InputName).h',
            'DLLDataFileName': 'dlldata.c',
            'InterfaceIdentifierFileName': '$(InputName)_i.c',
            'ProxyFileName': '$(InputName)_p.c',
          },
          'VCResourceCompilerTool': {
            'Culture' : '1033',
            'AdditionalIncludeDirectories': '$(SolutionDir)..',
          },
        },
      },
    }],
  ],
}
