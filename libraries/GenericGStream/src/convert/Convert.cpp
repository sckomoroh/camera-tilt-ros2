#include "Convert.h"

#include <cstdio>

#include <algorithm>
#include <string>
#include <vector>

#include <gst/gst.h>

GST_DEBUG_CATEGORY_STATIC(nnconvert_debug_category);
#define GST_CAT_DEFAULT nnconvert_debug_category

struct _NNConvertElementClass {
    GstBaseTransformClass parent_class;
};

struct _NNConvertElement {
    GstBaseTransform parent_instance;
    struct {
    } properties;
};

G_DEFINE_TYPE(NNConvertElement, nnconvert_element, GST_TYPE_BASE_TRANSFORM)

enum { PROP_0, N_PROPERTIES };

static GParamSpec* properties[N_PROPERTIES] = {nullptr};

static void nnconvert_element_init(NNConvertElement* self);
static gboolean nnconvert_element_start(GstBaseTransform* gstBaseTransform);
static GstFlowReturn nnconvert_element_transform_ip(GstBaseTransform* gstBaseTransform, GstBuffer* gstBuffer);
static gboolean plugin_init(GstPlugin* gstPlugin);
static void nnconvert_element_set_property(GObject* gObject,
                                             guint propertyId,
                                             const GValue* gValue,
                                             GParamSpec* gParamSpec);
static void nnconvert_element_get_property(GObject* gObject,
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

void nnconvert_element_class_init(NNConvertElementClass* klass) {
    GST_DEBUG_CATEGORY_INIT(nnconvert_debug_category, "nnconvert", 0, "NN convert plugin");

    GstElementClass* gstElementClass = GST_ELEMENT_CLASS(klass);
    GObjectClass* gObjectClass = G_OBJECT_CLASS(klass);

    auto gstBaseTransformClass = GST_BASE_TRANSFORM_CLASS(klass);

    gst_element_class_set_static_metadata(gstElementClass, "Ocrmentation postprocess DeepStream Element", "Filter/Meta",
                                          "Processes only metadata", "Yehor Zvihunov <yehor.zvihunov@globallogic.com>");

    gst_element_class_add_pad_template(gstElementClass, gst_static_pad_template_get(&sink_template));
    gst_element_class_add_pad_template(gstElementClass, gst_static_pad_template_get(&src_template));

    gstBaseTransformClass->transform_ip = GST_DEBUG_FUNCPTR(nnconvert_element_transform_ip);

    gstBaseTransformClass->passthrough_on_same_caps = true;
    gstBaseTransformClass->transform_ip_on_passthrough = true;
    gstBaseTransformClass->start = nnconvert_element_start;

    gObjectClass->set_property = nnconvert_element_set_property;
    gObjectClass->get_property = nnconvert_element_get_property;

    // properties[PROP_SOURCE_GIE_ID] = g_param_spec_uint("source-gie-id", "Source GIE ID", "Source GIE ID as uint",
    //                                                    0,    // Min
    //                                                    128,  // Max
    //                                                    0,    // Default
    //                                                    static_cast<GParamFlags>(G_PARAM_READWRITE));

    // g_object_class_install_property(gObjectClass, PROP_SOURCE_GIE_ID, properties[PROP_SOURCE_GIE_ID]);
}


void nnconvert_element_init(NNConvertElement* self) { GST_DEBUG_OBJECT(self, "init"); }

gboolean nnconvert_element_start(GstBaseTransform*) { return true; }

void nnconvert_element_set_property(GObject* gObject,
                                      guint propertyId,
                                      const GValue* gValue,
                                      GParamSpec* gParamSpec) {
    NNConvertElement* gstSelf = NNCONVERT_ELEMENT(gObject);

    switch (propertyId) {
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(gObject, propertyId, gParamSpec);
        break;
    }
}

void nnconvert_element_get_property(GObject* gObject, guint propertyId, GValue* gValue, GParamSpec* gParamSpec) {
    NNConvertElement* gstSelf = NNCONVERT_ELEMENT(gObject);

    switch (propertyId) {
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(gObject, propertyId, gParamSpec);
        break;
    }
}

GstFlowReturn nnconvert_element_transform_ip(GstBaseTransform* gstBaseTransform, GstBuffer* gstBuffer) {
    NNConvertElement* gstSelf = NNCONVERT_ELEMENT(gstBaseTransform);

    return GST_FLOW_OK;
}

gboolean plugin_init(GstPlugin* gstPlugin) {
    return gst_element_register(gstPlugin, "nnconvert", GST_RANK_NONE, NNCONVERT_TYPE_ELEMENT);
}

#define PACKAGE "nnpackage"

GST_PLUGIN_DEFINE(GST_VERSION_MAJOR,
                  GST_VERSION_MINOR,
                  nnconvert,
                  "NN convert plugin",
                  plugin_init,
                  "1.0",
                  "LGPL",
                  "GlobalLogic",
                  "https://example.com")
