#pragma once

#include "ControlWithLabel.h"

namespace reza { namespace ui {
    
template<typename T>
class RangeT : public ControlWithLabel
{
public:
    typedef std::shared_ptr<class RangeT<T>> RangeRef;
    typedef std::shared_ptr<class RangeT<int>> RangeiRef;
    typedef std::shared_ptr<class RangeT<float>> RangefRef;
    typedef std::shared_ptr<class RangeT<double>> RangedRef;
    
    struct Format {
    public:
        Format() { label(true).sticky(true).stickyValue(.1).precision(2); }
        Format( const Format &copy ) { mLabel = copy.mLabel; mSticky = copy.mSticky; mStickyValue = copy.mStickyValue; mPrecision = copy.mPrecision; }
        Format&	label( bool label = true ) { mLabel = label; return *this; }
        Format& sticky( bool sticky = true ) { mSticky = sticky; return *this; }
        Format& stickyValue( double value ) { mSticky = true; mStickyValue = value; return *this; }
        Format& precision( int precision ) { mPrecision = precision; return *this; }        
    protected:
        bool mLabel;
        bool mSticky;
        double mStickyValue;
        int mPrecision;        
        friend class RangeT;
    };
    
    static RangeRef create( const std::string name, T valueLow, T valueHigh, T min = 0.0f, T max = 1.0f, Format format = Format() )
    {
        return RangeRef( new RangeT<T>( name, valueLow, valueHigh, min, max, format ) );
    }
    
    static RangeRef create( const std::string name, T *valueLow, T *valueHigh, T min = 0.0, T max = 255.0, Format format = Format() )
    {
        return RangeRef( new RangeT<T>( name, valueLow, valueHigh, min, max, format ) );
    }
    
    const std::string getType() override { return "Range"; }
    
    void setValue( T valueLow, T valueHigh );
    
    void setValueLow( T value );
    void setValueHigh( T value );

    T getValueLow();
    T getValueHigh();

    float getNormalizedValueLow();
    float getNormalizedValueHigh();
    
    void updateValueRef();
    void updateLabel();
    
    virtual RangeT* setCallback( const std::function<void(T, T)> &callback );
    
    void setMax( T max, bool keepValueTheSame = false );
    T getMax();
    void setMin( T min, bool keepValueTheSame = false );
    T getMin();
    glm::vec2 getMinAndMax();
    void setMinAndMax( T min, T max, bool keepValueTheSame = false );
    void setValueLowRef( T *value );
    void setValueHighRef( T *value );
    
    bool isDraggable() override { return true; }
    
    RangeT( std::string name, T valueLow, T valueHigh, T min, T max, Format format = Format() );
    RangeT( std::string name, T *valueLow, T *valueHigh, T min, T max, Format format = Format() );
    virtual ~RangeT();
    
protected:
    void setup() override;
    void update() override;
    void trigger( bool recursive = false ) override;
    
    bool isSaveable() override { return true; }
    ci::JsonTree save() override;
    void load( const ci::JsonTree &data ) override;
    
    void drawFill( std::vector<RenderData> &data, const ci::ColorA &color ) override;
    void drawFillHighlight( std::vector<RenderData> &data, const ci::ColorA &color ) override;
    
    virtual void input( const glm::vec2 &pt );
    
    virtual void mouseDown( ci::app::MouseEvent &event ) override;
    virtual void mouseUp( ci::app::MouseEvent &event ) override;
    virtual void mouseWheel( ci::app::MouseEvent &event ) override;
    virtual void mouseMove( ci::app::MouseEvent &event ) override;
    virtual void mouseDrag( ci::app::MouseEvent &event ) override;
    
    virtual void keyDown( ci::app::KeyEvent &event ) override;
    virtual void keyUp( ci::app::KeyEvent &event ) override;
    
    T mMin;
    T mMax;
    double mValueLow;
    double mValueHigh;
    T *mValueLowRef;
    T *mValueHighRef;
    
    bool mUseLowRef;
    bool mUseHighRef;
    
    bool mHitHigh, mHitLow, mHitCenter;
    
    double mHitValueLow, mHitValueHigh;
    double mHitPoint;
    
    Format mFormat;
    bool mStickyEnabled;
    
    std::function<void(T, T)> mCallbackFn;
};

typedef RangeT<int> Rangei;
typedef RangeT<float> Rangef;
typedef RangeT<double> Ranged;

typedef std::shared_ptr<class RangeT<int>> RangeiRef;
typedef std::shared_ptr<class RangeT<float>> RangefRef;
typedef std::shared_ptr<class RangeT<double>> RangedRef;
    
} } //namespace reza::ui