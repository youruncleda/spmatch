
#pragma once

#include "image.hpp"
#include "grid.hpp"


/******************************************************************************
* > class StereoImage                                                         *
* An image and its additional informations used for stereo matching.          *
* The methods implements parts of the PatchMatch Stereo algorithm.            *
* Some methods requires this image to be bound to another one.                *
* The member disparityPlanes is the linear disparity function for each pixel. *
* See the comments in .cpp file                                               *
******************************************************************************/
class StereoImage {

	public:

		enum Side { LEFT, RIGHT };

	private:

		Image image;
		Image gradientX;
		Image gradientY;

		size_t width;
		size_t height;
		Grid<PlaneFunction> disparityPlanes;

		Side side;
		StereoImage* other = nullptr;	

	private:

		// private const methods
		double pixelDissimilarity(size_t w, size_t h,
				const PlaneFunction& disparity) const;
		double adaptiveWeight(size_t w1, size_t h1, size_t w2, size_t h2) const;
		double pixelWindowCost(size_t w, size_t h, const PlaneFunction& d) const;
		double disparityAt(size_t w, size_t h) const;
		Image getInvalidPixelsMap(void) const;
		Image wMedianFilterAtDisparity(const Image& disp, const Image& map) const;

		// private methods
		void fillInvalidPlanes(const Image& invalid);

	public:

		// constr
		StereoImage(const string& imgPath, Side side);
		explicit StereoImage(const StereoImage&) = default;
		StereoImage(StereoImage&&) = default;

		// const methods
		void displayGradients(void) const;
		pair<size_t, size_t> size(void) const { return { width, height }; }
		Image getDisparityMap(void) const;

		// methods
		void bind(StereoImage* o);
		void unbind(void);
		void setRandomDisparities(void);
		bool pixelSpatialPropagation(size_t w, size_t h, unsigned iteration);
		bool pixelViewPropagation(size_t w, size_t h);
		bool planeRefinement(size_t w, size_t h);
		Image processFinalDisparityMap(void);
};


/**************************************************************************
* > class StereoImagePair                                                 *
* Represents a couple of StereoImages. The method computeDisparity()      *
* returns two disparity images computed with PatchMatch Stereo algorithm. *
* See .cpp file                                                           *
**************************************************************************/
class StereoImagePair {
	
	private:

		StereoImage leftImg;
		StereoImage rightImg;

		size_t width;
		size_t height;

	public:

		// constr
		StereoImagePair(const string& leftImgPath, const string& rightImgPath);
		
		// methods
		pair<Image,Image> computeDisparity(void);

};
