/* Applied Video Analysis of Sequences (AVSA)
*
*	LAB2: Blob detection & classification
*	Lab2.0: Sample Opencv project
*
*
* Authors: José M. Martínez (josem.martinez@uam.es), Paula Moral (paula.moral@uam.es), Juan C. San Miguel (juancarlos.sanmiguel@uam.es)
*/

#include "blobs.hpp"
using namespace std;

/**
*	Draws blobs with different rectangles on the image 'frame'. All the input arguments must be
*  initialized when using this function.
*
* \param frame Input image
* \param pBlobList List to store the blobs found
* \param labelled - true write label and color bb, false does not wirite label nor color bb
*
* \return Image containing the draw blobs. If no blobs have to be painted
*  or arguments are wrong, the function returns a copy of the original "frame".
*
*/
Mat paintBlobImage(cv::Mat frame, std::vector<cvBlob> bloblist, bool labelled)
{
	cv::Mat blobImage;
	//check input conditions and return original if any is not satisfied
	//...
	frame.copyTo(blobImage);

	//required variables to paint
	//...

	//paint each blob of the list
	for (int i = 0; i < bloblist.size(); i++)
	{
		cvBlob blob = bloblist[i]; //get ith blob
		//...
		Scalar color;
		std::string label = "";
		switch (blob.label){
		case PERSON:
			color = Scalar(255, 0, 0);
			label = "PERSON";
			break;
		case CAR:
			color = Scalar(0, 255, 0);
			label = "CAR";
			break;
		case OBJECT:
			color = Scalar(0, 0, 255);
			label = "OBJECT";
			break;
		default:
			color = Scalar(255, 255, 255);
			label = "UNKOWN";
		}

		Point p1 = Point(blob.x, blob.y);
		Point p2 = Point(blob.x + blob.w, blob.y + blob.h);

		rectangle(blobImage, p1, p2, color, 1, 8, 0);
		if (labelled)
		{
			rectangle(blobImage, p1, p2, color, 1, 8, 0);
			putText(blobImage, label, p1, FONT_HERSHEY_SIMPLEX, 0.5, color);
		}
		else
			rectangle(blobImage, p1, p2, Scalar(255, 255, 255), 1, 8, 0);
	}

	//destroy all resources (if required)
	//...

	//return the image to show
	return blobImage;
}


/**
*	Blob extraction from 1-channel image (binary). The extraction is performed based
*	on the analysis of the connected components. All the input arguments must be
*  initialized when using this function.
*
* \param fgmask Foreground/Background segmentation mask (1-channel binary image)
* \param bloblist List with found blobs
*
* \return Operation code (negative if not succesfull operation)
*/
int extractBlobs(cv::Mat fgmask, std::vector<cvBlob> &bloblist, int connectivity)
{
	//check input conditions and return -1 if any is not satisfied
	//...		


	//required variables for connected component analysis 
	//...
	Mat aux; // image to be updated each time a blob is detected (blob cleared)
	fgmask.convertTo(aux, CV_32SC1);

	//clear blob list (to fill with this function)
	bloblist.clear();

	//Connected component analysis
	Mat fgmask_copy;
	Mat mask = Mat::zeros(Size(fgmask.cols + 2, fgmask.rows + 2), CV_8UC1);

	//to get rid of the shadows
	threshold(fgmask,fgmask,200,255,THRESH_BINARY);


	fgmask.copyTo(fgmask_copy);
	//newVal is the New value of the repainted domain pixels in the flood fill function
	int newVal = 255;

	int counter = 0;

	cv::Rect rect;
/*
	cout<<"fgmask.size() : "<<fgmask.size()<<endl;
	cout<<"fgmask_copy.size() : "<<fgmask_copy.size()<<endl;
	cout<<"fgmask.rows : "<<fgmask.rows<<endl;
	cout<<"fgmask.cols : "<<fgmask.cols<<endl;
	*/


	for (int i = 0; i < fgmask.rows; i++)
	{
		for (int j = 0; j < fgmask.cols; j++)
		{
			if (fgmask_copy.at<uchar>(j,i) == 255)
			{
				counter = counter + 1;

				cv::Point seed(j,i);
				//cout<<"seed : "<<seed<<endl;

				//cv::floodFill(fgmask_copy, mask, seed, newVal, &rect ,0, 0, connectivity);
				cv::floodFill(fgmask_copy, mask, seed, newVal, &rect ,0,0, connectivity|FLOODFILL_MASK_ONLY );

				//cv::floodFill(fgmask_copy,seed, newVal, &rect, 0, connectivity);
				cvBlob new_blob = initBlob(counter, rect.x, rect.y, rect.width, rect.height);

				bloblist.push_back(new_blob);

			}

		}
	}

	// void creation of a unqie blob in the center
	//	std::cout << bkg << " " << fg << " " << sh <<" " << fill << " " << unknown << " "<< bkg+fg+sh+unknown  << " " << fgmask.rows*fgmask.cols << std::endl;
	//	std::cout << blob_id << " " << small_blobs << std::endl;

	//destroy all resources
	//...

	//return OK code	 */
	return 1;
}


int removeSmallBlobs(std::vector<cvBlob> bloblist_in, std::vector<cvBlob> &bloblist_out, int min_width, int min_height)
{
	//check input conditions and return -1 if any is not satisfied

	//required variables
	//...

	//clear blob list (to fill with this function)
	bloblist_out.clear();

	for (int i = 0; i < bloblist_in.size(); i++)
	{
		cvBlob blob_in = bloblist_in[i]; //get ith blob

		// ...
		if (blob_in.w > min_width && blob_in.h > min_height )
		{
			bloblist_out.push_back(blob_in); // void implementation (does not remove)
		}
	}
	//destroy all resources
	//...

	//return OK code
	return 1;
}



/**
*	Blob classification between the available classes in 'Blob.hpp' (see CLASS typedef). All the input arguments must be
*  initialized when using this function.
*
* \param frame Input image
* \param fgmask Foreground/Background segmentation mask (1-channel binary image)
* \param bloblist List with found blobs
*
* \return Operation code (negative if not succesfull operation)
*/

// ASPECT RATIO MODELS
#define MEAN_PERSON 0.3950
#define STD_PERSON 0.1887

#define MEAN_CAR 1.4736
#define STD_CAR 0.2329

#define MEAN_OBJECT 1.2111
#define STD_OBJECT 0.4470

// end ASPECT RATIO MODELS

// distances
float ED(float val1, float val2)
{
	return sqrt(pow(val1 - val2, 2));
}

float WED(float val1, float val2, float std)
{
	return sqrt(pow(val1 - val2, 2) / pow(std, 2));
}
//end distances
int classifyBlobs(std::vector<cvBlob> &bloblist)
{
	//check input conditions and return -1 if any is not satisfied
	//...

	//required variables for classification
	//...

	//classify each blob of the list
	for (int i = 0; i < bloblist.size(); i++)
	{
		cvBlob blob = bloblist[i]; //get i-th blob
		//...

		float AR = float(blob.w)/float(blob.h);

		//float dist_person = ED(AR,MEAN_PERSON);
		//float dist_car = ED(AR,MEAN_CAR);
		//float dist_object = ED(AR,MEAN_OBJECT);

		float dist_person = WED(AR,MEAN_PERSON,STD_PERSON);
		float dist_car = WED(AR,MEAN_CAR,STD_CAR);
		float dist_object = WED(AR,MEAN_OBJECT,STD_OBJECT);


		if ( (AR >= MEAN_PERSON - STD_PERSON) && (AR <= MEAN_PERSON + STD_PERSON) && (dist_person < min(dist_car,dist_object) )   ){
			blob.label = PERSON;
		}
		else if ( (AR >= MEAN_CAR - STD_CAR) && (AR <= MEAN_CAR + STD_CAR) && (dist_car < min(dist_person,dist_object) )     ){
			blob.label = CAR;
		}
		else if  ( (AR >= MEAN_OBJECT - STD_OBJECT) && (AR <= MEAN_OBJECT + STD_OBJECT) && (dist_object < min(dist_person,dist_car) )     ){
			blob.label = OBJECT;
		}
		else {
			blob.label = UNKNOWN;
			// void implementation (does not change label -at creation UNKNOWN-)
		}

		bloblist[i] = blob;

	}


	//destroy all resources
	//...

	//return OK code
	return 1;
}

//stationary blob extraction function
/**
*	Stationary FG detection
*
* \param fgmask Foreground/Background segmentation mask (1-channel binary image)
* \param fgmask_history Foreground history counter image (1-channel integer image)
* \param sfgmask Foreground/Background segmentation mask (1-channel binary image)
*
* \return Operation code (negative if not succesfull operation)
*
*
* Based on: Stationary foreground detection for video-surveillance based on foreground and motion history images, D.Ortego, J.C.SanMiguel, AVSS2013
*
*/

#define FPS 25 //check in video - not really critical,frame rate
#define SECS_STATIONARY 5 // 1 5
#define I_COST 1 // 3 1 // increment cost for stationarity detection
#define D_COST 4// 2 4// decrement cost for stationarity detection
#define STAT_TH 0.5 // to set

int extractStationaryFG(Mat fgmask, Mat &fgmask_history, Mat &sfgmask)
{

	int numframes4static = (int)(FPS*SECS_STATIONARY);
	fgmask.convertTo(fgmask, CV_32FC1);
	Mat normalized_fgmask_history = Mat::zeros(Size(fgmask.cols, fgmask.rows), CV_32FC1);

	// the update step with the corresponding weights on the background and foreground pixels is done
	//fgmask_history = fgmask_history + I_COST * fgmask - D_COST * (255 - fgmask);
	//fgmask_history = max(0,fgmask_history);

	//normalized_fgmask_history = min (1, fgmask_history / numframes4static);

	// this history mask is thresholded and this determines the stationary foreground mask
	//sfgmask = normalized_fgmask_history >= STAT_TH;

	//a helper matrix
	//Mat ones = Mat::ones(fgmask_history.size(), CV_32FC1);
	// the temporary normalized version of the the history mask is crated
	//normalized_fgmask_history = min (ones, fgmask_history / numframes4static);

	// update fgmask_counter

	for (int i = 0; i<fgmask.rows; i++)
	{
		for (int j = 0; j<fgmask.cols; j++)
		{
		// increase FHI if it belongs to the foreground
			fgmask_history.at<float>(i,j) = fgmask_history.at<float>(i,j) + I_COST* (fgmask.at<float>(i,j)) - D_COST* (255-fgmask.at<float>(i,j)) ; // void implementation (no history)
			fgmask_history.at<float>(i,j) = max(float(0), fgmask_history.at<float>(i,j));

		// update sfgmask
			normalized_fgmask_history.at<float>(i,j) = min(float(1), fgmask_history.at<float>(i,j)/(255*numframes4static));

			if (normalized_fgmask_history.at<float>(i,j) >= STAT_TH)
			{
				sfgmask.at<uchar>(i,j) = 255;// void implementation (no stationary fg)
			}
			else
			{
				sfgmask.at<uchar>(i,j) = 0;
			}

		}

	}

	fgmask.convertTo(fgmask, CV_8UC1);

	return 1;
}



