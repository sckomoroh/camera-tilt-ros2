#pragma once

#include <gst/base/gstbasetransform.h>
#include <gst/gst.h>

G_BEGIN_DECLS

#define NNOVERLAY_TYPE_ELEMENT (nnoverlay_element_get_type())

G_DECLARE_FINAL_TYPE(NNOverlayElement, nnoverlay_element, MY, ELEMENT, GstBaseTransform)

#define NNOVERLAY_ELEMENT(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), NNOVERLAY_TYPE_ELEMENT, NNOverlayElement))

#define IS_NNOVERLAY_ELEMENT(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), NNOVERLAY_TYPE_ELEMENT))

G_END_DECLS
