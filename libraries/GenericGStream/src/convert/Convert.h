#pragma once

#include <gst/base/gstbasetransform.h>
#include <gst/gst.h>

G_BEGIN_DECLS

#define NNCONVERT_TYPE_ELEMENT (nnconvert_element_get_type())

G_DECLARE_FINAL_TYPE(NNConvertElement, nnconvert_element, MY, ELEMENT, GstBaseTransform)

#define NNCONVERT_ELEMENT(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), NNCONVERT_TYPE_ELEMENT, NNConvertElement))

#define IS_NNCONVERT_ELEMENT(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), NNCONVERT_TYPE_ELEMENT))

G_END_DECLS
