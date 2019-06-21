/* GStreamer
 * Copyright (C) 2019 FIXME <fixme@example.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Suite 500,
 * Boston, MA 02110-1335, USA.
 */
/**
 * SECTION:element-gstpdnraudio
 *
 * The pdnraudio element does FIXME stuff.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch-1.0 -v fakesrc ! pdnraudio ! FIXME ! fakesink
 * ]|
 * FIXME Describe what the pipeline does.
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/gst.h>
#include <gst/audio/gstaudiofilter.h>
#include "gstpdnraudio.h"

GST_DEBUG_CATEGORY_STATIC (gst_pd_nr_audio_debug_category);
#define GST_CAT_DEFAULT gst_pd_nr_audio_debug_category

/* prototypes */


static void gst_pd_nr_audio_set_property (GObject * object,
    guint property_id, const GValue * value, GParamSpec * pspec);
static void gst_pd_nr_audio_get_property (GObject * object,
    guint property_id, GValue * value, GParamSpec * pspec);
static void gst_pd_nr_audio_dispose (GObject * object);
static void gst_pd_nr_audio_finalize (GObject * object);

static gboolean gst_pd_nr_audio_setup (GstAudioFilter * filter,
    const GstAudioInfo * info);
static GstFlowReturn gst_pd_nr_audio_transform (GstBaseTransform * trans,
    GstBuffer * inbuf, GstBuffer * outbuf);
static GstFlowReturn gst_pd_nr_audio_transform_ip (GstBaseTransform * trans,
    GstBuffer * buf);

enum
{
  PROP_0
};

/* pad templates */

/* FIXME add/remove the formats that you want to support */
static GstStaticPadTemplate gst_pd_nr_audio_src_template =
GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("audio/x-raw,format=F32LE,rate=[1,max],"
      "channels=[1,max],layout=interleaved")
    );

/* FIXME add/remove the formats that you want to support */
static GstStaticPadTemplate gst_pd_nr_audio_sink_template =
GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("audio/x-raw,format=F32LE,rate=[1,max],"
      "channels=[1,max],layout=interleaved")
    );


/* class initialization */

G_DEFINE_TYPE_WITH_CODE (GstPdNrAudio, gst_pd_nr_audio, GST_TYPE_AUDIO_FILTER,
  GST_DEBUG_CATEGORY_INIT (gst_pd_nr_audio_debug_category, "pdnraudio", 0,
  "debug category for pdnraudio element"));

static void
gst_pd_nr_audio_class_init (GstPdNrAudioClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstBaseTransformClass *base_transform_class = GST_BASE_TRANSFORM_CLASS (klass);
  GstAudioFilterClass *audio_filter_class = GST_AUDIO_FILTER_CLASS (klass);

  /* Setting up pads and setting metadata should be moved to
     base_class_init if you intend to subclass this class. */
  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS(klass),
      &gst_pd_nr_audio_src_template);
  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS(klass),
      &gst_pd_nr_audio_sink_template);

  gst_element_class_set_static_metadata (GST_ELEMENT_CLASS(klass),
      "FIXME Long name", "Generic", "FIXME Description",
      "FIXME <fixme@example.com>");

  gobject_class->set_property = gst_pd_nr_audio_set_property;
  gobject_class->get_property = gst_pd_nr_audio_get_property;
  gobject_class->dispose = gst_pd_nr_audio_dispose;
  gobject_class->finalize = gst_pd_nr_audio_finalize;
  audio_filter_class->setup = GST_DEBUG_FUNCPTR (gst_pd_nr_audio_setup);
  base_transform_class->transform = GST_DEBUG_FUNCPTR (gst_pd_nr_audio_transform);
  base_transform_class->transform_ip = GST_DEBUG_FUNCPTR (gst_pd_nr_audio_transform_ip);

}

static void
gst_pd_nr_audio_init (GstPdNrAudio *pdnraudio)
{
  // pdnraudio->aFile = fopen( "audio.txt","w" );
  // pdnraudio->bFile = fopen( "source.txt","w" );

  pdnraudio->frequeny_sum_count = 0;
  pdnraudio->frequeny_data = g_new (GstFFTF32Complex, (FFT_WINDOW_SIZE/2)+2);

  pdnraudio->in_array = g_new0(gfloat, FFT_WINDOW_SIZE*2);
  pdnraudio->out_array = g_new0(gfloat, FFT_WINDOW_SIZE*2);


  g_print("gst_pd_nr_audio_init output_queue\n");

  pdnraudio->hop_offset = FFT_HOP_SIZE;
}

void
gst_pd_nr_audio_set_property (GObject * object, guint property_id,
    const GValue * value, GParamSpec * pspec)
{
  GstPdNrAudio *pdnraudio = GST_PD_NR_AUDIO (object);

  GST_DEBUG_OBJECT (pdnraudio, "set_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_pd_nr_audio_get_property (GObject * object, guint property_id,
    GValue * value, GParamSpec * pspec)
{
  GstPdNrAudio *pdnraudio = GST_PD_NR_AUDIO (object);

  GST_DEBUG_OBJECT (pdnraudio, "get_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_pd_nr_audio_dispose (GObject * object)
{
  GstPdNrAudio *pdnraudio = GST_PD_NR_AUDIO (object);

  GST_DEBUG_OBJECT (pdnraudio, "dispose");

  /* clean up as possible.  may be called multiple times */

  G_OBJECT_CLASS (gst_pd_nr_audio_parent_class)->dispose (object);
}

void
gst_pd_nr_audio_finalize (GObject * object)
{
  GstPdNrAudio *pdnraudio = GST_PD_NR_AUDIO (object);

  g_print("gst_pd_nr_audio_finalize\n");

  // GstFFTF32Complex *freq_data = pdnraudio->frequeny_data;
  // fprintf(pdnraudio->aFile,"const gfloat estR[] = {");
  // fprintf(pdnraudio->bFile,"const gfloat estI[] = {");
  // for(int j=0; j<FFT_SIZE; j++) {
  //   fprintf(pdnraudio->aFile,"%f ,", freq_data[j].r/pdnraudio->frequeny_sum_count);
  //   fprintf(pdnraudio->bFile,"%f ,", freq_data[j].i/pdnraudio->frequeny_sum_count);
  // }
  // fprintf(pdnraudio->aFile,"0};");
  // fprintf(pdnraudio->bFile,"0};");

  // fclose(pdnraudio->aFile);
  // fclose(pdnraudio->bFile);

  g_free(pdnraudio->frequeny_data);

  g_free(pdnraudio->in_array);
  g_free(pdnraudio->out_array);

  gst_fft_f32_free (pdnraudio->fft_ctx);


  GST_DEBUG_OBJECT (pdnraudio, "finalize");

  /* clean up object here */

  G_OBJECT_CLASS (gst_pd_nr_audio_parent_class)->finalize (object);
}

static gboolean
gst_pd_nr_audio_setup (GstAudioFilter * filter, const GstAudioInfo * info)
{
  GstPdNrAudio *pdnraudio = GST_PD_NR_AUDIO (filter);

  GST_DEBUG_OBJECT (pdnraudio, "setup");

  return TRUE;
}

guint trans_count = 0;

static void 
fft_window_noise_reduction (GstPdNrAudio *pdnraudio) {
  GstFFTF32 *fft_ctx = pdnraudio->fft_ctx;
  // GstFFTF32Complex *freq_data = pdnraudio->frequeny_data;
  GstFFTF32Complex *tmp_freq_data = g_new0 (GstFFTF32Complex, FFT_SIZE);

  gfloat *in_array = pdnraudio->in_array;
  gfloat *out_array = pdnraudio->out_array;

  gfloat *out_adata = g_new0 (gfloat, FFT_WINDOW_SIZE);

  for(int i=1; i<=4; i++) {
    gfloat *frame_data = g_new0 (gfloat, FFT_WINDOW_SIZE);
    memcpy(frame_data, in_array+FFT_HOP_SIZE*i, FFT_WINDOW_SIZE*sizeof(gfloat));

    // for(int j=0; j<FFT_WINDOW_SIZE; j++) {
    //   fprintf(pdnraudio->aFile,"%f\n", frame_data[j]);
    // }
    // fprintf(pdnraudio->aFile,"**************\n");

    fft_ctx = gst_fft_f32_new (FFT_WINDOW_SIZE, FALSE);

    gst_fft_f32_window (fft_ctx, frame_data, GST_FFT_WINDOW_HANN);
    gst_fft_f32_fft (fft_ctx, frame_data, tmp_freq_data);

    pdnraudio->frequeny_sum_count++;
    // todo: manipulate frequency domain here
    for(int j=0; j<FFT_SIZE; j++) {
      // freq_data[j].r += fabs(tmp_freq_data[j].r);
      // freq_data[j].i += fabs(tmp_freq_data[j].i);
      gfloat fbs_freq_data_r = fabs(tmp_freq_data[j].r);
      gfloat fbs_freq_data_i = fabs(tmp_freq_data[j].i);
      if((fbs_freq_data_r <= estR[j]*F_GAIN)||(fbs_freq_data_i <= estR[j]*F_GAIN)) {
        tmp_freq_data[j].r = 0;
        tmp_freq_data[j].i = 0;
      }

      // if(j == 100)
      //   g_print("r %f est %f \n", fabs(tmp_freq_data[j].r), estR[j]);
    }

    gst_fft_f32_free (fft_ctx);
    fft_ctx = gst_fft_f32_new (FFT_WINDOW_SIZE, TRUE);
    gst_fft_f32_inverse_fft(fft_ctx, tmp_freq_data, out_adata);

    for(int j=0; j<FFT_WINDOW_SIZE; j++) {
      out_array[j+FFT_HOP_SIZE*i] += (out_adata[j]/FFT_WINDOW_SIZE);
    }
    // fprintf(pdnraudio->aFile,"**************\n");

    gst_fft_f32_free (fft_ctx);
    g_free(frame_data);
  }

  // for(int j=0; j<FFT_WINDOW_SIZE; j++) {
  //   fprintf(pdnraudio->aFile,"%f\n", out_array[j]);
  // }

  g_free(out_adata);
  g_free(tmp_freq_data);
}

/* transform */
static GstFlowReturn
gst_pd_nr_audio_transform (GstBaseTransform * trans, GstBuffer * inbuf,
    GstBuffer * outbuf)
{
  GstPdNrAudio *pdnraudio = GST_PD_NR_AUDIO (trans);
  // guint rate = GST_AUDIO_FILTER_RATE (pdnraudio);
  // guint channels = GST_AUDIO_FILTER_CHANNELS (pdnraudio);
  // guint bps = GST_AUDIO_FILTER_BPS (pdnraudio);
  // guint bpf = GST_AUDIO_FILTER_BPF (pdnraudio);

  guint8 *in_data, *out_data;
  gsize in_size, out_size;
  gfloat *in_array = pdnraudio->in_array;
  gfloat *out_array = pdnraudio->out_array;

  GST_DEBUG_OBJECT (pdnraudio, "transform");

  GstMapInfo iinfo;
  if (!gst_buffer_map (inbuf, &iinfo ,GST_MAP_READ))
      return GST_FLOW_ERROR;

  GstMapInfo oinfo;
  if (!gst_buffer_map(outbuf, &oinfo, GST_MAP_WRITE))
      return GST_FLOW_ERROR;

  in_data = iinfo.data;
  in_size = iinfo.size;

  out_data = oinfo.data;
  out_size = oinfo.size;

  trans_count++;
  // g_print("nr_transform %d buffer size %ld rate %d channels %d bps %d bpf %d\n",
   // trans_count, in_size, rate, channels, bps, bpf);

  memcpy(in_array+FFT_WINDOW_SIZE, in_data, in_size);

  fft_window_noise_reduction(pdnraudio);

  memcpy(out_data, out_array, in_size);

  gfloat *tmp_out_array = g_new0 (gfloat, FFT_WINDOW_SIZE);

  // if(trans_count == 30) {
  //   for(int j=0; j<FFT_WINDOW_SIZE*2; j++) {
  //     fprintf(pdnraudio->aFile,"%f\n", out_array[j]);
  //   }
  // }

  memcpy(tmp_out_array, out_array+FFT_WINDOW_SIZE, FFT_WINDOW_SIZE*sizeof(gfloat));
  memset(out_array+FFT_WINDOW_SIZE, 0, FFT_WINDOW_SIZE*sizeof(gfloat));
  memcpy(out_array, tmp_out_array, FFT_WINDOW_SIZE*sizeof(gfloat));

  g_free(tmp_out_array);

  memcpy(in_array, in_data, in_size);

  gst_buffer_unmap(inbuf, &iinfo);
  gst_buffer_unmap(outbuf, &oinfo);

  return GST_FLOW_OK;
}

static GstFlowReturn
gst_pd_nr_audio_transform_ip (GstBaseTransform * trans, GstBuffer * buf)
{
  GstPdNrAudio *pdnraudio = GST_PD_NR_AUDIO (trans);

  GST_DEBUG_OBJECT (pdnraudio, "transform_ip");

  g_print("gst_pd_nr_audio_transform_ip\n");

  return GST_FLOW_OK;
}

static gboolean
plugin_init (GstPlugin * plugin)
{
  GST_DEBUG_CATEGORY_INIT (gst_pd_nr_audio_debug_category, "pdnraudio", 0,
      "PlayDate audio noise reduction plugin example");

  /* FIXME Remember to set the rank if it's an element that is meant
     to be autoplugged by decodebin. */
  return gst_element_register (plugin, "pdnraudio", GST_RANK_NONE,
      GST_TYPE_PD_NR_AUDIO);
}

/* FIXME: these are normally defined by the GStreamer build system.
   If you are creating an element to be included in gst-plugins-*,
   remove these, as they're always defined.  Otherwise, edit as
   appropriate for your external plugin package. */
#ifndef VERSION
#define VERSION "0.0.1"
#endif
#ifndef PACKAGE
#define PACKAGE "NoiseReduction"
#endif
#ifndef PACKAGE_NAME
#define PACKAGE_NAME "PlayDate"
#endif
#ifndef GST_PACKAGE_ORIGIN
#define GST_PACKAGE_ORIGIN "http://www.startplaydate.com/"
#endif

GST_PLUGIN_DEFINE (
    GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    pdnraudio,
    "PlayDate audio noise reduction plugin description",
    plugin_init, VERSION, "LGPL", PACKAGE_NAME, GST_PACKAGE_ORIGIN)

