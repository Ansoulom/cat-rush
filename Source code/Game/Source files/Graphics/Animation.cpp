#include "Animation.h"
#include "Texture.h"
#include "Resource_managers.h"
#include <chrono>


namespace Game
{
	namespace Graphics
	{
		Animation::Animation(std::string texture, int frames, int rows, double frame_rate, bool looping)
			: texture_name_{texture},
			  frames_{frames},
			  rows_{rows},
			  current_frame_{0},
			  current_row_{0},
			  frame_rate_{frame_rate},
			  timer_{Timer::Seconds{1.0} / frame_rate},
			  looping_{looping}
		{
			assert(frames >= 1);
			assert(rows >= 1);
			assert(frame_rate > 0);
		}


		Animation::Frame Animation::current_frame(const Resources::Texture_manager& tm) const
		{
			const auto& texture = tm.get_texture(texture_name_);
			const auto width = texture.width() / frames_;
			const auto height = texture.height() / rows_;
			const auto clip = Geometry::Rectangle<int>{
				{current_frame_ * width, current_row_ * height},
				width,
				height,
				Geometry::Pivot_point::top_left
			};

			return {texture, clip};
		}


		bool Animation::update(Timer::Seconds time_passed)
		{
			if(finished_) return false;

			if(timer_.update(time_passed))
			{
				timer_.reset_to_surplus();
				if(++current_frame_ >= frames_)
				{
					if(looping_)
						current_frame_ = 0;
					else
					{
						current_frame_ = frames_ - 1;
						finished_ = true;
						return true;
					}
				}
			}
			return false;
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


		Animation Animation::from_json(const Io::json& json)
		{
			const auto texture = json.at("texture").get<std::string>();
			const auto frames = json.at("frames").get<int>();
			const auto rows = json.at("rows").get<int>();
			const auto frame_rate = json.at("frame_rate").get<double>();
			const auto looping = json.at("looping").get<bool>();

			return {texture, frames, rows, frame_rate, looping};
		}


		Io::json Animation::to_json() const
		{
			return {
				{"texture", texture_name_},
				{"frames", frames_},
				{"rows", rows_},
				{"frame_rate", frame_rate_},
				{"looping", looping_}
			};
		}


		Animation::Frame::Frame(const Texture& texture, const Geometry::Rectangle<int>& clip)
			: texture_{&texture}, clip_{clip}
		{
			if(clip.left() < 0 || clip.top() < 0 || clip.right() > texture.width() || clip.bottom() > texture.height())
			{
				throw std::runtime_error{"Clip can not be bigger than or outside of texture."};
			}
		}


		const Texture& Animation::Frame::texture() const
		{
			return *texture_;
		}


		Geometry::Rectangle<int> Animation::Frame::clip() const
		{
			return clip_;
		}


		void Animation::reset()
		{
			timer_.reset();
			finished_ = false;
			current_frame_ = 0;
		}
	}
}
