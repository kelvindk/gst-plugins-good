gstpdnraudio = library('gstpdnraudio',
  'gstpdnraudio.c',
  c_args : gst_plugins_bad_args,
  include_directories : [configinc],
  dependencies : [gstbase_dep, gstfft_dep, gstaudio_dep, libm],
  install : true,
  install_dir : plugins_install_dir,
)
