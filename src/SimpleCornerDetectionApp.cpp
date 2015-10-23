#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"

#include "cinder/Rand.h"
#include "cinder/Capture.h"

#include "CinderOpenCv.h"

#include "markerdetector.h"
#include "cvdrawingutils.h"

using namespace ci;
using namespace ci::app;

class SimpleCornerDetectionApp : public App {
  public:
	void setup();
	void update();
	void draw();
    
    gl::TextureRef          mInput;
    SurfaceRef             	mSurf;
    CaptureRef              mCapture;
    
    //aruco::CameraParameters mCamParam;
    aruco::MarkerDetector mMarkerDetector;
    std::vector<aruco::Marker> mMarkers;
};

void SimpleCornerDetectionApp::setup()
{
    setFrameRate( 60.0 );
    setWindowSize( 640, 480 );
	// CAMERA
    try {
		mCapture = Capture::create( 640, 480 );
		mCapture->start();
	}catch( ... ) {
		console() << "Failed to initialize capture" << std::endl;
	}
}   
void SimpleCornerDetectionApp::update(){
    if( mCapture->checkNewFrame() ){
        mSurf = mCapture->getSurface();
        mInput = gl::Texture::create( *mSurf );
    }
    if( mSurf ){
        cv::Mat input = toOcv( *mSurf );
        mMarkerDetector.detect( input, mMarkers );
    }
}
void SimpleCornerDetectionApp::draw()
{

	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    gl::enableAlphaBlending();
    gl::color( ColorA(1,1,1,1.0f) );
    if( mInput ){
        gl::draw( mInput );
    }
    gl::color( ColorA(1,1,1,0.7f) );
    for( int i=0; i<mMarkers.size(); i++){
        for( int ii = 0; ii < mMarkers[i].size(); ii++){
            gl::drawSolidCircle(vec2(mMarkers[i][ii].x, mMarkers[i][ii].y), 10.0f);
        }
    }
}

CINDER_APP( SimpleCornerDetectionApp, RendererGl )
