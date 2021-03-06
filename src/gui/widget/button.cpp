// button.cpp
/*
  neogfx C++ GUI Library
  Copyright (c) 2015 Leigh Johnston.  All Rights Reserved.
  
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

#include <neogfx/neogfx.hpp>
#include <neogfx/gui/widget/button.hpp>
#include <neogfx/gui/widget/label.hpp>
#include <neogfx/app/app.hpp>

namespace neogfx
{
	button::button(alignment aAlignment) :
		widget{}, iCheckable{ button_checkable::NotCheckable }, iCheckedState{ false }, iLayout{ *this }, iLabel{ iLayout, label_type::MultiLine, aAlignment }
	{
		init();
	}

	button::button(const std::string& aText, alignment aAlignment) :
		widget{}, iCheckable{ button_checkable::NotCheckable }, iCheckedState{ false }, iLayout{ *this }, iLabel{ iLayout, aText, label_type::MultiLine, aAlignment }
	{
		init();
	}
	
	button::button(const i_texture& aTexture, alignment aAlignment) :
		widget{}, iCheckable{ button_checkable::NotCheckable }, iCheckedState{ false }, iLayout{ *this }, iLabel{ iLayout, aTexture, label_type::MultiLine, aAlignment }
	{
		init();
	}

	button::button(const i_image& aImage, alignment aAlignment) :
		widget{}, iCheckable{ button_checkable::NotCheckable }, iCheckedState{ false }, iLayout{ *this }, iLabel{ iLayout, aImage, label_type::MultiLine, aAlignment }
	{
		init();
	}

	button::button(i_widget& aParent, alignment aAlignment) :
		widget{ aParent }, iCheckable{ button_checkable::NotCheckable }, iCheckedState{ false }, iLayout{ *this }, iLabel{ iLayout, label_type::MultiLine, aAlignment }
	{
		init();
	}

	button::button(i_widget& aParent, const std::string& aText, alignment aAlignment) :
		widget{ aParent }, iCheckable{ button_checkable::NotCheckable }, iCheckedState{ false }, iLayout{ *this }, iLabel{ iLayout, aText, label_type::MultiLine, aAlignment }
	{
		init();
	}

	button::button(i_widget& aParent, const i_texture& aTexture, alignment aAlignment) :
		widget{ aParent }, iCheckable{ button_checkable::NotCheckable }, iCheckedState{ false }, iLayout{ *this }, iLabel{ iLayout, aTexture, label_type::MultiLine, aAlignment }
	{
		init();
	}

	button::button(i_widget& aParent, const i_image& aImage, alignment aAlignment) :
		widget{ aParent }, iCheckable{ button_checkable::NotCheckable }, iCheckedState{ false }, iLayout{ *this }, iLabel{ iLayout, aImage, label_type::MultiLine, aAlignment }
	{
		init();
	}

	button::button(i_layout& aLayout, alignment aAlignment) :
		widget{ aLayout }, iCheckable{ button_checkable::NotCheckable }, iCheckedState{ false }, iLayout{ *this }, iLabel{ iLayout, label_type::MultiLine, aAlignment }
	{
		init();
	}

	button::button(i_layout& aLayout, const std::string& aText, alignment aAlignment) :
		widget{ aLayout }, iCheckable{ button_checkable::NotCheckable }, iCheckedState{ false }, iLayout{ *this }, iLabel{ iLayout, aText, label_type::MultiLine, aAlignment }
	{
		init();
	}

	button::button(i_layout& aLayout, const i_texture& aTexture, alignment aAlignment) :
		widget{ aLayout }, iCheckable{ button_checkable::NotCheckable }, iCheckedState{ false }, iLayout{ *this }, iLabel{ iLayout, aTexture, label_type::MultiLine, aAlignment }
	{
		init();
	}

	button::button(i_layout& aLayout, const i_image& aImage, alignment aAlignment) :
		widget{ aLayout }, iCheckable{ button_checkable::NotCheckable }, iCheckedState{ false }, iLayout{ *this }, iLabel{ iLayout, aImage, label_type::MultiLine, aAlignment }
	{
		init();
	}

	button::~button()
	{
		app::instance().remove_mnemonic(*this);
	}

	neogfx::size_policy button::size_policy() const
	{
		if (widget::has_size_policy())
			return widget::size_policy();
		return neogfx::size_policy{neogfx::size_policy::Expanding, neogfx::size_policy::Minimum};
	}

	margins button::margins() const
	{
		neogfx::margins result = widget::margins();
		if (!has_margins())
		{
			result.left *= 2.0;
			result.right *= 2.0;
		}
		return result;
	}

	button_checkable button::checkable() const
	{
		return iCheckable;
	}

	void button::set_checkable(button_checkable aCheckable)
	{
		iCheckable = aCheckable;
	}

	bool button::is_checked() const
	{
		return iCheckedState != std::nullopt && *iCheckedState == true;
	}

	bool button::is_unchecked() const
	{
		return iCheckedState != std::nullopt && *iCheckedState == false;
	}

	bool button::is_indeterminate() const
	{
		return iCheckedState == std::nullopt;
	}

	void button::check()
	{
		set_checked_state(true);
	}

	void button::uncheck()
	{
		set_checked_state(false);
	}

	void button::set_indeterminate()
	{
		set_checked_state(std::nullopt);
	}

	void button::set_checked(bool aChecked)
	{
		set_checked_state(aChecked);
	}

	void button::toggle()
	{
		if (!can_toggle())
			return;
		if (is_checked() || is_indeterminate())
			set_checked(false);
		else
			set_checked(true);
	}

	const neogfx::label& button::label() const
	{
		return iLabel;
	}

	neogfx::label& button::label()
	{
		return iLabel;
	}

	const image_widget& button::image() const
	{
		return label().image();
	}

	image_widget& button::image()
	{
		return label().image();
	}

	const text_widget& button::text() const
	{
		return label().text();
	}

	text_widget& button::text()
	{
		return label().text();
	}

	void button::mouse_button_pressed(mouse_button aButton, const point& aPosition, key_modifiers_e aKeyModifiers)
	{
		widget::mouse_button_pressed(aButton, aPosition, aKeyModifiers);
		if (aButton == mouse_button::Left)
		{
			update();
			pressed.trigger();
		}
	}

	void button::mouse_button_double_clicked(mouse_button aButton, const point& aPosition, key_modifiers_e aKeyModifiers)
	{
		widget::mouse_button_double_clicked(aButton, aPosition, aKeyModifiers);
		if (aButton == mouse_button::Left)
		{
			update();
			double_clicked.trigger();
		}
	}

	void button::mouse_button_released(mouse_button aButton, const point& aPosition)
	{
		bool wasCapturing = capturing();
		widget::mouse_button_released(aButton, aPosition);
		if (aButton == mouse_button::Left)
		{
			update();
			if (wasCapturing)
			{
				destroyed_flag destroyed{ *this };
				if (client_rect().contains(aPosition))
					handle_clicked();
				if (!destroyed)
					released.trigger();
			}
		}
		else if (aButton == mouse_button::Right)
		{
			if (wasCapturing && client_rect().contains(aPosition))
				right_clicked.trigger();
		}
	}

	bool button::key_pressed(scan_code_e aScanCode, key_code_e, key_modifiers_e)
	{
		if (aScanCode == ScanCode_SPACE)
		{
			handle_clicked();
			return true;
		}
		return false;
	}

	void button::handle_clicked()
	{
		destroyed_flag destroyed{ *this };
		clicked.trigger();
		async_clicked.async_trigger();
		if (!destroyed && iCheckable != button_checkable::NotCheckable)
			toggle();
	}

	bool button::can_toggle() const
	{
		return true;
	}

	const std::optional<bool>& button::checked_state() const
	{
		return iCheckedState;
	}

	bool button::set_checked_state(const std::optional<bool>& aCheckedState)
	{
		if (iCheckedState == aCheckedState)
			return false;
		if (aCheckedState == std::nullopt && iCheckable != button_checkable::TriState)
			throw not_tri_state_checkable();
		iCheckedState = aCheckedState;
		update();
		toggled.trigger();
		if (is_checked())
			checked.trigger();
		else if (is_unchecked())
			unchecked.trigger();
		else if (is_indeterminate())
			indeterminate.trigger();
		return true;
	}

	std::string button::mnemonic() const
	{
		return mnemonic_from_text(label().text().text());
	}

	void button::mnemonic_execute()
	{
		handle_clicked();
	}

	i_widget& button::mnemonic_widget()
	{
		return label().text();
	}

	void button::init()
	{
		clicked.set_trigger_type(event_trigger_type::Asynchronous);
		double_clicked.set_trigger_type(event_trigger_type::Asynchronous);

		layout().set_margins(neogfx::margins(0.0));
		iLabel.set_size_policy(neogfx::size_policy::Expanding);

		set_focus_policy(focus_policy::TabFocus);

		auto label_text_updated = [this]()
		{
			auto m = mnemonic_from_text(label().text().text());
			if (!m.empty())
				app::instance().add_mnemonic(*this);
			else
				app::instance().remove_mnemonic(*this);
		};
		iSink = label().text().text_changed(label_text_updated);
		label_text_updated();
	}
}


