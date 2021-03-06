// i_property.hpp
/*
  neogfx C++ GUI Library
  Copyright (c) 2018 Leigh Johnston.  All Rights Reserved.
  
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

#pragma once

#include <neogfx/neogfx.hpp>
#include <any>
#include <neolib/variant.hpp>
#include <neogfx/core/event.hpp>
#include <neogfx/core/geometrical.hpp>
#include <neogfx/core/colour.hpp>
#include <neogfx/gfx/text/font.hpp>
#include <neogfx/gui/widget/widget_bits.hpp>

namespace neogfx
{
	class custom_type : public std::any
	{
	public:
		using any::any;
	};

	typedef neolib::variant<
		void*,
		bool,
		char,
		int32_t,
		uint32_t,
		int64_t,
		uint64_t,
		float,
		double,
		std::string,
		size,
		point,
		rect,
		custom_type> variant_t;

	class property_variant : public variant_t
	{
	public:
		property_variant() : 
			variant_t{}
		{
		}
		property_variant(const property_variant& other) :
			variant_t{ other }
		{
		}
		property_variant(property_variant&& other) :
			variant_t{ std::move(other) }
		{
		}
		template <typename T>
		property_variant(T&& aValue) :
			variant_t{ std::forward<T>(aValue) }
		{
		}
	public:
		using variant_t::operator==;
		using variant_t::operator!=;
	};

	template <typename T> struct variant_type_for { typedef custom_type type; };
	template <typename T> struct variant_type_for<T*> { typedef void* type; };

	class i_property
	{
	public:
		event<property_variant> changed;
	public:
		virtual ~i_property() {}
	public:
		virtual const std::string& name() const = 0;
		virtual const std::type_info& type() const = 0;
		virtual const std::type_info& category() const = 0;
		virtual bool optional() const = 0;
		virtual property_variant get() const = 0;
		virtual void set(const property_variant& aValue) = 0;
	};
}
