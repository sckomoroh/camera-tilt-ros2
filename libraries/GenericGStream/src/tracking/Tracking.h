#pragma once

#include <gst/base/gstbasetransform.h>
#include <gst/gst.h>

G_BEGIN_DECLS

#define NNTRACKING_TYPE_ELEMENT (nntracking_element_get_type())

G_DECLARE_FINAL_TYPE(NNTrackingElement, nntracking_element, MY, ELEMENT, GstBaseTransform)

#define NNTRACKING_ELEMENT(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), NNTRACKING_TYPE_ELEMENT, NNTrackingElement))

#define IS_NNTRACKING_ELEMENT(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), NNTRACKING_TYPE_ELEMENT))

G_END_DECLS
