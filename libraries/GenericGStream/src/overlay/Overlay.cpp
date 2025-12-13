#include "Overlay.h"

#include <cstdio>

#include <algorithm>
#include <string>
#include <vector>

#include <gst/gst.h>

GST_DEBUG_CATEGORY_STATIC(nnoverlay_debug_category);
#define GST_CAT_DEFAULT nnoverlay_debug_category

struct _NNOverlayElementClass {
    GstBaseTransformClass parent_class;
};

struct _NNOverlayElement {
    GstBaseTransform parent_instance;
    struct {
    } properties;
};

G_DEFINE_TYPE(NNOverlayElement, nnoverlay_element, GST_TYPE_BASE_TRANSFORM)

enum { PROP_0, N_PROPERTIES };

static GParamSpec* properties[N_PROPERTIES] = {nullptr};

static void nnoverlay_element_init(NNOverlayElement* self);
static gboolean nnoverlay_element_start(GstBaseTransform* gstBaseTransform);
static GstFlowReturn nnoverlay_element_transform_ip(GstBaseTransform* gstBaseTransform, GstBuffer* gstBuffer);
static gboolean plugin_init(GstPlugin* gstPlugin);
static void nnoverlay_element_set_property(GObject* gObject,
                                             guint propertyId,
                                             const GValue* gValue,
                                             GParamSpec* gParamSpec);
static void nnoverlay_element_get_property(GObject* gObject,
                                             guint propertyId,
                                             GValue* gValue,
                                             GParamSpec* gParamSpec);

// TODO: Change me
static GstStaticPadTemplate sink_template =
    GST_STATIC_PAD_TEMPLATE("sink",
                            GST_PAD_SINK,
                            GST_PAD_ALWAYS,
                            GST_STATIC_CAPS("video/x-raw;video/x-raw(memory:NVMM)"));

static GstStaticPadTemplate src_template =
    GST_STATIC_PAD_TEMPLATE("src",
                            GST_PAD_SRC,
                            GST_PAD_ALWAYS,
                            GST_STATIC_CAPS("video/x-raw;video/x-raw(memory:NVMM)"));

void nnoverlay_element_class_init(NNOverlayElementClass* klass) {
    GST_DEBUG_CATEGORY_INIT(nnoverlay_debug_category, "nnoverlay", 0, "NN overlay plugin");

    GstElementClass* gstElementClass = GST_ELEMENT_CLASS(klass);
    GObjectClass* gObjectClass = G_OBJECT_CLASS(klass);

    auto gstBaseTransformClass = GST_BASE_TRANSFORM_CLASS(klass);

    gst_element_class_set_static_metadata(gstElementClass, "Ocrmentation postprocess DeepStream Element", "Filter/Meta",
                                          "Processes only metadata", "Yehor Zvihunov <yehor.zvihunov@globallogic.com>");

    gst_element_class_add_pad_template(gstElementClass, gst_static_pad_template_get(&sink_template));
    gst_element_class_add_pad_template(gstElementClass, gst_static_pad_template_get(&src_template));

    gstBaseTransformClass->transform_ip = GST_DEBUG_FUNCPTR(nnoverlay_element_transform_ip);

    gstBaseTransformClass->passthrough_on_same_caps = true;
    gstBaseTransformClass->transform_ip_on_passthrough = true;
    gstBaseTransformClass->start = nnoverlay_element_start;

    gObjectClass->set_property = nnoverlay_element_set_property;
    gObjectClass->get_property = nnoverlay_element_get_property;

    // properties[PROP_SOURCE_GIE_ID] = g_param_spec_uint("source-gie-id", "Source GIE ID", "Source GIE ID as uint",
    //                                                    0,    // Min
    //                                                    128,  // Max
    //                                                    0,    // Default
    //                                                    static_cast<GParamFlags>(G_PARAM_READWRITE));

    // g_object_class_install_property(gObjectClass, PROP_SOURCE_GIE_ID, properties[PROP_SOURCE_GIE_ID]);
}


void nnoverlay_element_init(NNOverlayElement* self) { GST_DEBUG_OBJECT(self, "init"); }

gboolean nnoverlay_element_start(GstBaseTransform*) { return true; }

void nnoverlay_element_set_property(GObject* gObject,
                                      guint propertyId,
                                      const GValue* gValue,
                                      GParamSpec* gParamSpec) {
    NNOverlayElement* gstSelf = NNOVERLAY_ELEMENT(gObject);

    switch (propertyId) {
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(gObject, propertyId, gParamSpec);
        break;
    }
}

void nnoverlay_element_get_property(GObject* gObject, guint propertyId, GValue* gValue, GParamSpec* gParamSpec) {
    NNOverlayElement* gstSelf = NNOVERLAY_ELEMENT(gObject);

    switch (propertyId) {
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(gObject, propertyId, gParamSpec);
        break;
    }
}

GstFlowReturn nnoverlay_element_transform_ip(GstBaseTransform* gstBaseTransform, GstBuffer* gstBuffer) {
    NNOverlayElement* gstSelf = NNOVERLAY_ELEMENT(gstBaseTransform);

    return GST_FLOW_OK;
}

gboolean plugin_init(GstPlugin* gstPlugin) {
    return gst_element_register(gstPlugin, "nnoverlay", GST_RANK_NONE, NNOVERLAY_TYPE_ELEMENT);
}

#define PACKAGE "nnpackage"

GST_PLUGIN_DEFINE(GST_VERSION_MAJOR,
                  GST_VERSION_MINOR,
                  nnoverlay,
                  "NN overlay plugin",
                  plugin_init,
                  "1.0",
                  "LGPL",
                  "GlobalLogic",
                  "https://example.com")

