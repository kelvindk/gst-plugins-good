plugin_LTLIBRARIES = libgstpdnraudio.la

libgstpdnraudio_la_SOURCES = gstspectrum.c
libgstpdnraudio_la_CFLAGS = $(GST_PLUGINS_BASE_CFLAGS) $(GST_BASE_CFLAGS) \
	$(GST_CFLAGS)
libgstpdnraudio_la_LIBADD = $(GST_PLUGINS_BASE_LIBS) \
	-lgstfft-$(GST_API_VERSION) -lgstaudio-$(GST_API_VERSION) \
	$(GST_BASE_LIBS) $(GST_LIBS) $(LIBM)
libgstpdnraudio_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS)

noinst_HEADERS = gstpdnraudio.h
