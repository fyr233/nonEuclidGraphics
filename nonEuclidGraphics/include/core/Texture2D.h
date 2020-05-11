#pragma once
/*
openCV: 修改工程的包含目录、库目录、附加依赖项
*/

#include <core/vec.h>
#include <core/mat.h>
#include <core/Mesh.h>
#include <core/transform.h>

#include <opencv2/opencv.hpp>

namespace cgcore
{
	class Texture2D
	{
	public:
		Texture2D();
		Texture2D(const std::string& path);
		~Texture2D();

		void Load(const std::string& path);
		vecf3 Sample(vecf2 uv);

	private:


	public:
		enum class WrapMode { Clamp, Repeat, Mirror };
		enum class SampleMode { Nearest, Linear };

		cv::Mat img;

	private:

	};

	Texture2D::Texture2D()
	{
	}

	inline Texture2D::Texture2D(const std::string& path)
	{
		Load(path);
	}

	Texture2D::~Texture2D()
	{
	}

	inline void Texture2D::Load(const std::string& path)
	{
		img = cv::imread(path, cv::IMREAD_COLOR);//3 channel BGR
		if (img.empty())
		{
			std::cout << "ERROR::Texture2D::Load:" << std::endl
				<< "\t" << "open file (" << path << ") fail" << std::endl;
		}
	}

	vecf3 Texture2D::Sample(vecf2 uv)//u, v in [0,1)
	{
		if (img.empty())
		{
			std::cout << "ERROR::Texture2D::Sample:" << std::endl
				<< "\t" << "img is empty" << std::endl;
			return vecf3({ 0, 1, 0 });
		}

		int xidx = int(uv[0] * img.cols);
		int yidx = int(uv[1] * img.rows);
		float xlambda = uv[0] * img.cols - xidx;
		float ylambda = uv[1] * img.rows - yidx;
		//双线性插值
		cv::Vec3b result = (1 - ylambda) * ((1 - xlambda) * img.at<cv::Vec3b>(yidx, xidx) + xlambda * img.at<cv::Vec3b>(yidx, (xidx + 1) % img.cols))
			+ ylambda * ((1 - xlambda) * img.at<cv::Vec3b>((yidx + 1) % img.rows, xidx) + xlambda * img.at<cv::Vec3b>((yidx + 1) % img.rows, (xidx + 1) % img.cols));

		return vecf3({(float)result[2], (float)result[1] , (float)result[0] });//转换为RGB
	}
}