#pragma once

namespace reza {
namespace ui {

template <class T>
class PaddingT {
  public:
	PaddingT( T padding )
		: mTop( padding ), mRight( padding ), mBottom( padding ), mLeft( padding ) {}
	PaddingT( T top, T right, T bottom, T left )
		: mTop( top ), mRight( right ), mBottom( bottom ), mLeft( left ) {}
	PaddingT()
		: mTop( 0 ), mRight( 0 ), mBottom( 0 ), mLeft( 0 ) {}
	T mTop;
	T mBottom;
	T mLeft;
	T mRight;
};
typedef PaddingT<float> Paddingf;
typedef PaddingT<int> Paddingi;
}
} // namespace reza::ui