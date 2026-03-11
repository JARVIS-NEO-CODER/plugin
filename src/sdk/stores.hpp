#pragma once

#include <cassert>

#include "scssdk_telemetry.h"

namespace ets2la_plugin
{

	SCSAPI_VOID telemetry_store_dplacement(const scs_string_t name, const scs_u32_t index, const scs_value_t* const value, const scs_context_t context)
	{
		// The SCS_TELEMETRY_CHANNEL_FLAG_no_value flag was not provided during registration
		// so this callback is only called when a valid value is available.

		assert(value);
		assert(value->type == SCS_VALUE_TYPE_dplacement);
		assert(context);
		static_cast<scs_value_dplacement_t*>(context)->position = value->value_dplacement.position;
		static_cast<scs_value_dplacement_t*>(context)->orientation = value->value_dplacement.orientation;
	}

}
