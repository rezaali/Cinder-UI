#pragma once 

#include "ControlWithLabel.h"

namespace reza { namespace ui {
    
template<typename T>
class SliderT : public ControlWithLabel
{
public:
    typedef std::shared_ptr<class SliderT<T>> SliderRef;
    typedef std::shared_ptr<class SliderT<int>> SlideriRef;
    typedef std::shared_ptr<class SliderT<float>> SliderfRef;
    typedef std::shared_ptr<class SliderT<double>> SliderdRef;

    struct Format {
    public:
        Format() { label(true).sticky(false).stickyValue(.1).precision(2).crossFader(false); }
        Format( const Format &copy ) {
            mLabel = copy.mLabel;
            mSticky = copy.mSticky;
            mStickyValue = copy.mStickyValue;
            mPrecision = copy.mPrecision;
            mCrossFader = copy.mCrossFader; 
        }
        Format&	label( bool label = true ) { mLabel = label; return *this; }
        Format& sticky( bool sticky ) { mSticky = sticky; return *this; }
        Format& stickyValue( double value ) { mStickyValue = value; return *this; }
        Format& precision( int precision ) { mPrecision = precision; return *this; }
        Format& crossFader( bool crossFader = true ){ mCrossFader = crossFader; return *this; }
    protected:
        bool mCrossFader;
        bool mLabel;
        bool mSticky;
        double mStickyValue;
        int mPrecision;
        friend class SliderT;
    };
    
    static SliderRef create( const std::string name, T val, T min = 0.0, T max = 255, Format format = Format() )
    {
        return SliderRef( new SliderT<T>( name, val, min, max, format ) );
    }
    
    static SliderRef create( const std::string name, T *val, T min = 0.0, T max = 255, Format format = Format() )
    {
        return SliderRef( new SliderT<T>( name, val, min, max, format ) );
    }

    SliderT( std::string name, T value, T min, T max, Format format = Format() );
    SliderT( std::string name, T *value, T min, T max, Format format = Format() );
    virtual ~SliderT();

    const std::string getType() override { return "Slider"; }
    
    void setValue( T value );
    T getValue();
    double getNormalizedValue();
    
    virtual SliderT* setCallback( const std::function<void(T)> &callback );

    void setMax( T max, bool keepValueTheSame = false );
    T getMax();
    void setMin( T min, bool keepValueTheSame = false );
    T getMin();
    glm::vec2 getMaxAndMin();
    void setMinAndMax( T min, T max, bool keepValueTheSame = false );
    void setValueRef( T *value );
    
    bool isDraggable() override { return true; }

protected:
    virtual void setup() override;
    void update() override;
    void trigger( bool recursive = false ) override;
    void updateValueRef();
    void updateLabel();
    
    bool isSaveable() override { return true; }
    ci::JsonTree save() override;
    void load( const ci::JsonTree &data ) override;
    
    void drawFill( std::vector<RenderData> &data, const ci::ColorA &color ) override; 
    void drawFillHighlight( std::vector<RenderData> &data, const ci::ColorA &color ) override;

    virtual void input( ci::app::MouseEvent &event );
    
    virtual void mouseDown( ci::app::MouseEvent &event ) override;
    virtual void mouseUp( ci::app::MouseEvent &event ) override;
    virtual void mouseWheel( ci::app::MouseEvent &event ) override;
    virtual void mouseMove( ci::app::MouseEvent &event ) override;
    virtual void mouseDrag( ci::app::MouseEvent &event ) override;
    
    virtual void keyDown( ci::app::KeyEvent &event ) override;
    virtual void keyUp( ci::app::KeyEvent &event ) override;
    
    T mMin;
    T mMax;
    double mValue;
    T *mValueRef;
    bool mUseRef;    
    Format mFormat;
    std::function<void(T)> mCallbackFn;
};
    
typedef SliderT<int> Slideri;
typedef SliderT<float> Sliderf;
typedef SliderT<double> Sliderd;
    
typedef std::shared_ptr<class SliderT<int>> SlideriRef;
typedef std::shared_ptr<class SliderT<float>> SliderfRef;
typedef std::shared_ptr<class SliderT<double>> SliderdRef;
    
} } //namespace reza::ui