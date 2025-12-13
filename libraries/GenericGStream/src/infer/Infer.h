#pragma once

#include <gst/base/gstbasetransform.h>
#include <gst/gst.h>

G_BEGIN_DECLS

#define NNINFER_TYPE_ELEMENT (nninfer_element_get_type())

G_DECLARE_FINAL_TYPE(NNInferElement, nninfer_element, MY, ELEMENT, GstBaseTransform)

#define NNINFER_ELEMENT(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), NNINFER_TYPE_ELEMENT, NNInferElement))

#define IS_NNINFER_ELEMENT(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), NNINFER_TYPE_ELEMENT))

G_END_DECLS
