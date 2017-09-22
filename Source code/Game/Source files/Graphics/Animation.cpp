#include "Animation.h"
#include "Texture.h"
#include "Texture_manager.h"


namespace Game
{
	namespace Graphics
	{
		Animation::Animation(std::string texture, int frames, int rows, double frame_rate) : texture_name_{texture}, frames_{frames}, rows_{rows}, current_frame_{0}, current_row_{0}, frame_rate_{frame_rate}, timer_{Timer::Seconds{1.0} / frame_rate}
		{
			if(frames < 1)
			{
				throw std::out_of_range{"Must have at least one frame."};
			}
			if(rows < 1)
			{
				throw std::out_of_range{"Must have at least one row."};
			}
			if(frame_rate < std::numeric_limits<double>::epsilon()) // Is this the correct usage of epsilon?
			{
				throw std::out_of_range{"Frame rate must be larger than zero."};
			}
		}


		Animation::~Animation()
		{
		}


		Animation::Frame Animation::get_current_frame(const Texture_manager& tm) const
		{
			auto texture = tm.get_texture(texture_name_);
			auto width = texture->get_width() / frames_;
			auto height = texture->get_height() / rows_;
			auto clip = Geometry::Rectangle<int>{{current_frame_ * width, current_row_ * height}, width, height, Geometry::Pivot_point::top_left};

			return {*texture, clip};
		}


		void Animation::update(Timer::Seconds time_passed)
		{
			if(timer_.update(time_passed))
			{
				timer_.reset_to_surplus();
				if(++current_frame_ >= frames_)
				{
					current_frame_ = 0;
				}
			}
		}


		void Animation::set_row(int row)
		{
			if(row >= rows_)
			{
				throw std::out_of_range{"Row number cannot exceed the number of rows."};
			}
			if(row < 0)
			{
				throw std::out_of_range{"Row number must be greater than zero."};
			}
			current_row_ = row;
		}


		Animation Animation::from_json(const nlohmann::json& json)
		{
			auto texture = json.at("texture").get<std::string>();
			auto frames = json.at("frames").get<int>();
			auto rows = json.at("rows").get<int>();
			auto frame_rate = json.at("frame_rate").get<double>();

			return {texture, frames, rows, frame_rate};
		}


		nlohmann::json Animation::to_json() const
		{
			return {{"texture", texture_name_},{"frames", frames_},{"rows", rows_},{"frame_rate", frame_rate_}};
		}


		Animation::Frame::Frame(Texture& texture, const Geometry::Rectangle<int>& clip) : texture_{texture}, clip_{clip}
		{
			if(clip.left() < 0 || clip.top() < 0 || clip.right() > texture.get_width() || clip.bottom() > texture.get_height())
			{
				throw std::runtime_error{"Clip can not be bigger than or outside of texture."};
			}
		}


		Texture& Animation::Frame::get_texture() const
		{
			return texture_;
		}


		Geometry::Rectangle<int> Animation::Frame::get_clip() const
		{
			return clip_;
		}
	}
}
