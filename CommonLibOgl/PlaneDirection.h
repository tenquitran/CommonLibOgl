#pragma once


namespace CommonLibOgl
{
	// Direction of the plane.
	enum class EPlaneDirection {
		Undefined,
		HorizontalYPositive,    /* horizontal, normal has the same direction as the positive Y half-axis */
		HorizontalYNegative,    /* horizontal, normal has the same direction as the negative Y half-axis */
		VerticalXPositive,      /* vertical, normal has the same direction as the positive X half-axis */
		VerticalXNegative       /* vertical, normal has the same direction as the negative X half-axis */
	};
}
