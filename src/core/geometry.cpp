// geometry.cpp
/*
  neogfx C++ GUI Library
  Copyright(C) 2015-present Leigh Johnston
  
  This program is free software: you can redistribute it and / or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <neogfx/core/geometry.hpp>

namespace neogfx
{ 
	units_context::units_context(const i_units_context& aSource) :
		iSource{ aSource },
		iUnits{ units::Pixels }
	{
	}

	bool units_context::device_metrics_available() const
	{
		return iSource.device_metrics_available();
	}

	const i_device_metrics& units_context::device_metrics() const
	{
		return iSource.device_metrics();
	}

	units units_context::units() const
	{
		return iUnits;
	}

	units units_context::set_units(neogfx::units aUnits) const
	{
		neogfx::units oldUnits = iUnits;
		iUnits = aUnits;
		return oldUnits;
	}
	
	units_converter::units_converter(const i_units_context& aContext) :
		iContext(aContext), iSavedUnits(aContext.units())
	{
	}

	units_converter::~units_converter()
	{
		iContext.set_units(iSavedUnits);
	}

	units units_converter::saved_units() const
	{
		return iSavedUnits;
	}

	units units_converter::units() const
	{
		return iContext.units();
	}

	units units_converter::set_units(neogfx::units aUnits) const
	{
		return iContext.set_units(aUnits);
	}

	vector2 units_converter::to_device_units(const vector2& aValue) const
	{
		return iContext.device_metrics_available() ? to_device_units(iContext.device_metrics().extents(), aValue) : 
			units() == units::Pixels ? aValue : throw device_metrics_unavailable();
	}

	dimension units_converter::to_device_units(dimension aValue) const
	{
		return iContext.device_metrics_available() ? to_device_units(iContext.device_metrics().extents(), size{ aValue, 0 }).cx : 
			units() == units::Pixels ? aValue : throw device_metrics_unavailable();
	}

	delta units_converter::to_device_units(const delta& aValue) const
	{
		return iContext.device_metrics_available() ? to_device_units(iContext.device_metrics().extents(), aValue) : 
			units() == units::Pixels ? aValue : throw device_metrics_unavailable();
	}

	size units_converter::to_device_units(const size& aValue) const
	{
		return iContext.device_metrics_available() ? to_device_units(iContext.device_metrics().extents(), aValue) : 
			units() == units::Pixels ? aValue : throw device_metrics_unavailable();
	}

	point units_converter::to_device_units(const point& aValue) const
	{
		return iContext.device_metrics_available() ? to_device_units(iContext.device_metrics().extents(), aValue) : 
			units() == units::Pixels ? aValue : throw device_metrics_unavailable();
	}

	rect units_converter::to_device_units(const rect& aValue) const
	{
		return iContext.device_metrics_available() ? to_device_units(iContext.device_metrics().extents(), aValue) : 
			units() == units::Pixels ? aValue : throw device_metrics_unavailable();
	}

	margins units_converter::to_device_units(const margins& aValue) const
	{
		return iContext.device_metrics_available() ? margins{
			to_device_units(iContext.device_metrics().extents(), size{ aValue.left, 0 }).cx,
			to_device_units(iContext.device_metrics().extents(), size{ 0, aValue.top }).cy,
			to_device_units(iContext.device_metrics().extents(), size{ aValue.right, 0 }).cx,
			to_device_units(iContext.device_metrics().extents(), size{ 0, aValue.bottom }).cy } :
			units() == units::Pixels ? aValue : throw device_metrics_unavailable();
	}

	vector2 units_converter::to_device_units(const size& aExtents, const vector2& aValue) const
	{
		if (!iContext.device_metrics_available() && units() != units::Pixels && units() != units::Percentage)
			throw device_metrics_unavailable();
		switch (units())
		{
		default:
		case units::Pixels:
			return aValue;
		case units::Points:
			return aValue * vector2((1.0 / 72.0) * static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), (1.0 / 72.0) * static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Picas:
			return aValue * vector2((1.0 / 6.0) * static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), (1.0 / 6.0) * static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Ems:
			return aValue * vector2(1.0 * static_cast<dimension>(iContext.device_metrics().em_size()) * static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), 1.0 * static_cast<dimension>(iContext.device_metrics().em_size()) * static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Millimetres:
			return aValue * vector2((1.0 / 25.4) * static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), (1.0 / 25.4) * static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Centimetres:
			return aValue * vector2((1.0 / 2.54) * static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), (1.0 / 2.54) * static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Inches:
			return aValue * vector2(1.0 * static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), 1.0 * static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Percentage:
			return vector2(aExtents.cx, aExtents.cy) * vector2(aValue[0] / 100.0, aValue[1] / 100.0);
		}
	}

	dimension units_converter::to_device_units(const size& aExtents, dimension aValue) const
	{
		if (!iContext.device_metrics_available() && units() != units::Pixels && units() != units::Percentage)
			throw device_metrics_unavailable();
		return to_device_units(aExtents, size{ aValue, 0 }).cx;
	}

	delta units_converter::to_device_units(const size& aExtents, const delta& aValue) const
	{
		if (!iContext.device_metrics_available() && units() != units::Pixels && units() != units::Percentage)
			throw device_metrics_unavailable();
		switch (units())
		{
		default:
		case units::Pixels:
			return aValue;
		case units::Points:
			return aValue * delta((1.0/72.0) * static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), (1.0/72.0) * static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Picas:
			return aValue * delta((1.0/6.0) * static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), (1.0/6.0) * static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Ems:
			return aValue * delta(1.0 * static_cast<dimension>(iContext.device_metrics().em_size()) * static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), 1.0 * static_cast<dimension>(iContext.device_metrics().em_size()) * static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Millimetres:
			return aValue * delta((1.0/25.4) * static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), (1.0/25.4) * static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Centimetres:
			return aValue * delta((1.0/2.54) * static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), (1.0/2.54) * static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Inches:
			return aValue * delta(1.0 * static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), 1.0 * static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Percentage:
			return delta(aExtents.to_delta()) * delta(aValue.dx / 100.0, aValue.dy / 100.0);
		}
	}

	size units_converter::to_device_units(const size& aExtents, const size& aValue) const
	{
		if (!iContext.device_metrics_available() && units() != units::Pixels && units() != units::Percentage)
			throw device_metrics_unavailable();
		switch (units())
		{
		default:
		case units::Pixels:
			return aValue;
		case units::Points:
			return aValue * size((1.0/72.0) * static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), (1.0/72.0) * static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Picas:
			return aValue * size((1.0/6.0) * static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), (1.0/6.0) * static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Ems:
			return aValue * size(1.0 * static_cast<dimension>(iContext.device_metrics().em_size()) * static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), 1.0 * static_cast<dimension>(iContext.device_metrics().em_size()) * static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Millimetres:
			return aValue * size((1.0/25.4) * static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), (1.0/25.4) * static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Centimetres:
			return aValue * size((1.0/2.54) * static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), (1.0/2.54) * static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Inches:
			return aValue * size(1.0 * static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), 1.0 * static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Percentage:
			return size(aExtents) * size(aValue.cx / 100.0, aValue.cy / 100.0);
		}
	}

	point units_converter::to_device_units(const size& aExtents, const point& aValue) const
	{
		if (!iContext.device_metrics_available() && units() != units::Pixels && units() != units::Percentage)
			throw device_metrics_unavailable();
		switch (units())
		{
		default:
		case units::Pixels:
			return aValue;
		case units::Points:
			return aValue * point((1.0/72.0) * static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), (1.0/72.0) * static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Picas:
			return aValue * point((1.0/6.0) * static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), (1.0/6.0) * static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Ems:
			return aValue * point(1.0 * static_cast<dimension>(iContext.device_metrics().em_size()) * static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), 1.0 * static_cast<dimension>(iContext.device_metrics().em_size()) * static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Millimetres:
			return aValue * point((1.0/25.4) * static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), (1.0/25.4) * static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Centimetres:
			return aValue * point((1.0/2.54) * static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), (1.0/2.54) * static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Inches:
			return aValue * point(1.0 * static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), 1.0 * static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Percentage:
			return point(size(aExtents)) * point(aValue.x / 100.0, aValue.y / 100.0);
		}
	}

	rect units_converter::to_device_units(const size& aExtents, const rect& aValue) const
	{
		if (!iContext.device_metrics_available() && units() != units::Pixels && units() != units::Percentage)
			throw device_metrics_unavailable();
		return rect(to_device_units(aExtents, aValue.position()), to_device_units(aExtents, aValue.extents()));
	}

	vector2 units_converter::from_device_units(const vector2& aValue) const
	{
		return iContext.device_metrics_available() ? from_device_units(iContext.device_metrics().extents(), aValue) :
			units() == units::Pixels ? aValue : throw device_metrics_unavailable();
	}

	dimension units_converter::from_device_units(dimension aValue) const
	{
		return iContext.device_metrics_available() ? from_device_units(iContext.device_metrics().extents(), size{ aValue, 0 }).cx :
			units() == units::Pixels ? aValue : throw device_metrics_unavailable();
	}

	delta units_converter::from_device_units(const delta& aValue) const
	{
		return iContext.device_metrics_available() ? from_device_units(iContext.device_metrics().extents(), aValue) :
			units() == units::Pixels ? aValue : throw device_metrics_unavailable();
	}

	size units_converter::from_device_units(const size& aValue) const
	{
		return iContext.device_metrics_available() ? from_device_units(iContext.device_metrics().extents(), aValue) :
			units() == units::Pixels ? aValue : throw device_metrics_unavailable();
	}

	point units_converter::from_device_units(const point& aValue) const
	{
		return iContext.device_metrics_available() ? from_device_units(iContext.device_metrics().extents(), aValue) :
			units() == units::Pixels ? aValue : throw device_metrics_unavailable();
	}

	rect units_converter::from_device_units(const rect& aValue) const
	{
		return iContext.device_metrics_available() ? from_device_units(iContext.device_metrics().extents(), aValue) :
			units() == units::Pixels ? aValue : throw device_metrics_unavailable();
	}

	margins units_converter::from_device_units(const margins& aValue) const
	{
		return iContext.device_metrics_available() ? margins{
			from_device_units(iContext.device_metrics().extents(), size{ aValue.left, 0 }).cx,
			from_device_units(iContext.device_metrics().extents(), size{ 0, aValue.top }).cy,
			from_device_units(iContext.device_metrics().extents(), size{ aValue.right, 0 }).cx,
			from_device_units(iContext.device_metrics().extents(), size{ 0, aValue.bottom }).cy } :
			units() == units::Pixels ? aValue : throw device_metrics_unavailable();
	}

	vector2 units_converter::from_device_units(const size& aExtents, const vector2& aValue) const
	{
		if (!iContext.device_metrics_available() && units() != units::Pixels && units() != units::Percentage)
			throw device_metrics_unavailable();
		switch (units())
		{
		default:
		case units::Pixels:
			return aValue;
		case units::Points:
			return vector2(aValue) * vector2(72.0 / static_cast<scalar>(iContext.device_metrics().horizontal_dpi()), 72.0 / static_cast<scalar>(iContext.device_metrics().vertical_dpi()));
		case units::Picas:
			return vector2(aValue) * vector2(6.0 / static_cast<scalar>(iContext.device_metrics().horizontal_dpi()), 6.0 / static_cast<scalar>(iContext.device_metrics().vertical_dpi()));
		case units::Ems:
			return vector2(aValue) * vector2((1.0 / static_cast<scalar>(iContext.device_metrics().em_size())) / static_cast<scalar>(iContext.device_metrics().horizontal_dpi()), (1.0 / static_cast<dimension>(iContext.device_metrics().em_size())) / static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Millimetres:
			return vector2(aValue) * vector2(25.4 / static_cast<scalar>(iContext.device_metrics().horizontal_dpi()), 25.4 / static_cast<scalar>(iContext.device_metrics().vertical_dpi()));
		case units::Centimetres:
			return vector2(aValue) * vector2(2.54 / static_cast<scalar>(iContext.device_metrics().horizontal_dpi()), 2.54 / static_cast<scalar>(iContext.device_metrics().vertical_dpi()));
		case units::Inches:
			return vector2(aValue) * vector2(1.0 / static_cast<scalar>(iContext.device_metrics().horizontal_dpi()), 1.0 / static_cast<scalar>(iContext.device_metrics().vertical_dpi()));
		case units::Percentage:
			return vector2(aValue) / vector2(aExtents.cx, aExtents.cy) * vector2(100.0, 100.0);
		}
	}

	dimension units_converter::from_device_units(const size& aExtents, dimension aValue) const
	{
		if (!iContext.device_metrics_available() && units() != units::Pixels && units() != units::Percentage)
			throw device_metrics_unavailable();
		return from_device_units(aExtents, size{aValue, 0.0}).cx;
	}

	delta units_converter::from_device_units(const size& aExtents, const delta& aValue) const
	{
		if (!iContext.device_metrics_available() && units() != units::Pixels && units() != units::Percentage)
			throw device_metrics_unavailable();
		switch (units())
		{
		default:
		case units::Pixels:
			return aValue;
		case units::Points:
			return delta(aValue) * delta(72.0 / static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), 72.0 / static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Picas:
			return delta(aValue) * delta(6.0 / static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), 6.0 / static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Ems:
			return delta(aValue) * delta((1.0 / static_cast<dimension>(iContext.device_metrics().em_size())) / static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), (1.0 / static_cast<dimension>(iContext.device_metrics().em_size())) / static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Millimetres:
			return delta(aValue) * delta(25.4 / static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), 25.4 / static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Centimetres:
			return delta(aValue) * delta(2.54 / static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), 2.54 / static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Inches:
			return delta(aValue) * delta(1.0 / static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), 1.0 / static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Percentage:
			return delta(aValue) / delta(aExtents.to_delta()) * delta(100.0, 100.0);
		}
	}

	size units_converter::from_device_units(const size& aExtents, const size& aValue) const
	{
		if (!iContext.device_metrics_available() && units() != units::Pixels && units() != units::Percentage)
			throw device_metrics_unavailable();
		switch (units())
		{
		default:
		case units::Pixels:
			return aValue;
		case units::Points:
			return size(aValue) * size(72.0 / static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), 72.0 / static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Picas:
			return size(aValue) * size(6.0 / static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), 6.0 / static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Ems:
			return size(aValue) * size((1.0 / static_cast<dimension>(iContext.device_metrics().em_size())) / static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), (1.0 / static_cast<dimension>(iContext.device_metrics().em_size())) / static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Millimetres:
			return size(aValue) * size(25.4 / static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), 25.4 / static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Centimetres:
			return size(aValue) * size(2.54 / static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), 2.54 / static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Inches:
			return size(aValue) * size(1.0 / static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), 1.0 / static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Percentage:
			return size(aValue) / size(aExtents) * size(100.0, 100.0);
		}
	}

	point units_converter::from_device_units(const size& aExtents, const point& aValue) const
	{
		if (!iContext.device_metrics_available() && units() != units::Pixels && units() != units::Percentage)
			throw device_metrics_unavailable();
		switch (units())
		{
		default:
		case units::Pixels:
			return aValue;
		case units::Points:
			return point(aValue) * point(72.0 / static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), 72.0 / static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Picas:
			return point(aValue) * point(6.0 / static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), 6.0 / static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Ems:
			return point(aValue) * point((1.0 / static_cast<dimension>(iContext.device_metrics().em_size())) / static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), (1.0 / static_cast<dimension>(iContext.device_metrics().em_size())) / static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Millimetres:
			return point(aValue) * point(25.4 / static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), 25.4 / static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Centimetres:
			return point(aValue) * point(2.54 / static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), 2.54 / static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Inches:
			return point(aValue) * point(1.0 / static_cast<dimension>(iContext.device_metrics().horizontal_dpi()), 1.0 / static_cast<dimension>(iContext.device_metrics().vertical_dpi()));
		case units::Percentage:
			return point(aValue) / point(aExtents) * point(100.0, 100.0);
		}
	}

	rect units_converter::from_device_units(const size& aExtents, const rect& aValue) const
	{
		if (!iContext.device_metrics_available() && units() != units::Pixels && units() != units::Percentage)
			throw device_metrics_unavailable();
		return rect(from_device_units(aExtents, aValue.position()), from_device_units(aExtents, aValue.extents()));
	}
}