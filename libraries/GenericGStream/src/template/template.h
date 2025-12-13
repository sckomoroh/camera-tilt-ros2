#pragma once

#include <gst/base/gstbasetransform.h>
#include <gst/gst.h>

G_BEGIN_DECLS

#define NN_T_TYPE_ELEMENT (nn_t_element_get_type())

G_DECLARE_FINAL_TYPE(NN_TElement, nn_t_element, MY, ELEMENT, GstBaseTransform)

#define NN_T_ELEMENT(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), NN_T_TYPE_ELEMENT, NN_TElement))

#define IS_NN_T_ELEMENT(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), NN_T_TYPE_ELEMENT))

G_END_DECLS
