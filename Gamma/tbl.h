#ifndef GAMMA_TBL_H_INC
#define GAMMA_TBL_H_INC

/*	Gamma - Generic processing library
	See COPYRIGHT file for authors and license information */
 
#include <stdlib.h>
#include "Gamma/arr.h"
#include "Gamma/mem.h"
#include "Gamma/scl.h"
#include "Gamma/Constants.h"

#define TEM template<class T>
#define LOOP(n,s) for(uint32_t i=0; i<n; i+=s)

namespace gam{


/// Window types
enum WindowType{ 	
	BARTLETT,			/**< Bartlett (Triangle) */
	BLACKMAN,			/**< Blackman */
	BLACKMAN_HARRIS,	/**< Blackman-Harris */
	HAMMING,			/**< Hamming */
	HANN,				/**< von Hann */
	WELCH,				/**< Welch */
	NYQUIST,			/**< Nyquist */
	RECTANGLE			/**< Rectangle (no window) */
};

/// Returns human readable string of window type
inline static const char * toString(WindowType v){
	#define CS(name) case name: return #name;
	switch(v){
		CS(BARTLETT) CS(BLACKMAN) CS(BLACKMAN_HARRIS) CS(HAMMING) CS(HANN)
		CS(WELCH) CS(NYQUIST) CS(RECTANGLE)
		default: return "Unknown";
	}
	#undef CS
}



/// Add sine wave to array

/// @param[in] dst		destination array
/// @param[in] len		length of array
/// @param[in] cycles	number of cycles of sine wave
/// @param[in] amp		amplitude of sine wave
/// @param[in] phs		phase of sine wave, in [0,1]
template <class T>
void addSine(T * dst, uint32_t len, double cycles=1, double amp=1, double phs=0);

template <class T, template<class> class ArrayType>
void inline addSine(ArrayType<T>& dst, double cycles=1, double amp=1, double phs=0){
	addSine(&dst[0], dst.size(), cycles, amp, phs);
}


/// Add harmonic series to array with specified amplitudes

/// @param[in] dst		destination array
/// @param[in] len		length of destination array
/// @param[in] amps		amplitudes of harmonic series, size must be hNum
/// @param[in] hNum		total number of harmonics
/// @param[in] hMul		harmonic number multiplication factor
/// @param[in] hShf		harmonic number shift amount
/// @param[in] phs		phase of sine wave, in [0,1]
template <class T, class A>
void inline addSines(
	T * dst, uint32_t len, const A * amps, int hNum, int hMul=1, int hShf=1, double phs=0
){
	for(int i=0; i<hNum; ++i){
		if(A(0) != amps[i]) addSine(dst,len,i*hMul+hShf,amps[i],phs);
	}
}
template <class T, template<class> class ArrayType, class A>
void inline addSines(ArrayType<T>& dst, const A * amps, int hNum, int hMul=1, int hShf=1, double phs=0){
	addSines(&dst[0],dst.size(), amps, hNum,hMul,hShf,phs);
}


/// Add harmonics to array with specified amplitudes and harmonic numbers

/// @param[in] dst		destination array
/// @param[in] len		length of destination array
/// @param[in] amps		harmonic amplitudes of series, size must be hNum
/// @param[in] cycs		harmonic numbers of series, size must be hNum
/// @param[in] hNum		total number of harmonics
/// @param[in] phs		phase of sine wave, in [0,1]
template <class T, class A, class C>
void addSines(
	T * dst, uint32_t len, const A * amps, const C * cycs, int hNum, double phs=0
){
	for(int i=0; i<hNum; ++i) addSine(dst,len,cycs[i],amps[i],phs);
}
template <class T, template<class> class ArrayType, class A, class C>
void inline addSines(ArrayType<T>& dst, const A * amps, const C * cycs, int hNum, double phs=0){
	addSines(&dst[0],dst.size(), amps,cycs,hNum,phs);
}


/// Add multiple sine waves to array

/// \tparam InvPower
/// @param[in] dst		destination array
/// @param[in] len		length of destination array
/// @param[in] amps		amplitudes of harmonic series, size must be hNum
/// @param[in] hNum		total number of harmonics
/// @param[in] hMul		harmonic number multiplication factor
/// @param[in] hShf		harmonic number shift amount
/// @param[in] phs		phase of sine wave, in [0,1]
template <int InvPower, class T>
void addSinesPow(T * dst, uint32_t len, int hNum, int hMul=1, int hShf=1, double amp=1, double phs=0);

template <int InvPower, class T, template<class> class ArrayType>
inline void addSinesPow(ArrayType<T>& dst, int hNum, int hMul=1, int hShf=1, double amp=1, double phs=0){
	addSinesPow<InvPower>(&dst[0], dst.size(), hNum,hMul,hShf,amp,phs);
}


/// Table functions
namespace tbl{


/// Fills array with one period of a cosine wave.
TEM void cosine(T * dst, uint32_t len);

/// Fills array with one period of a sine wave.
TEM void sine(T * dst, uint32_t len);

/// Fills array with arbitrary phase and length sinusoid.
TEM void sinusoid(T * dst, uint32_t len, double phase, double periods);

/// Sums band-limited impulse wave into multi-wavetable array

/// The waveform includes harmonics in the range [hrmLo, hrmHi].
/// The amplitude of the waveform will not be normalized.
/// The ideal waveform shape is [4, -1, 0, -1, 0, -1, 0, -1 ]
TEM void multiImpulse(T * dst, uint32_t len, uint32_t hrmLo, uint32_t hrmHi);

/// Sums band-limited saw wave into multi-wavetable array

/// The waveform includes harmonics in the range [hrmLo, hrmHi].
/// The ideal waveform shape is [1, 0.75, 0.5, 0.25, 0, -0.25, -0.5, -0.75]
TEM void multiSaw(T * dst, uint32_t len, uint32_t hrmLo, uint32_t hrmHi);

/// Sums band-limited square wave into multi-wavetable array

/// The waveform includes harmonics in the range [hrmLo, hrmHi].
///	The ideal waveform shape is [ 1, 1, 1, 1, -1, -1, -1, -1].
TEM void multiSquare(T * dst, uint32_t len, uint32_t hrmLo, uint32_t hrmHi);

/// Sums band-limited triangle wave into multi-wavetable array
	
/// The waveform includes harmonics in the range [hrmLo, hrmHi].
///	The ideal waveform shape is [ 0, 0.5, 1, 0.5, 0, -0.5, -1, -0.5].
TEM void multiTriangle(T * dst, uint32_t len, uint32_t hrmLo, uint32_t hrmHi);

/// Create multi-wavetable
TEM void multiWave(T * dst, uint32_t len, uint32_t order, void (* func)(T *, uint32_t, uint32_t, uint32_t));

/// Returns maximum number of harmonics that will fit in array.
inline uint32_t maxHarmonics(uint32_t len){ return len>>1; }

/// Fills array with specified window type
TEM void window			(T * dst, uint32_t len, WindowType type);
TEM void bartlett		(T * dst, uint32_t len); ///< Fills array with Bartlett window
TEM void blackman		(T * dst, uint32_t len); ///< Fills array with Blackman window
TEM void blackmanHarris	(T * dst, uint32_t len); ///< Fills array with Blackman-Harris window
TEM void hamming		(T * dst, uint32_t len); ///< Fills array with Hamming window
TEM void hann			(T * dst, uint32_t len); ///< Fills array with von Hann window
TEM void welch			(T * dst, uint32_t len); ///< Fills array with Welch window
TEM void rectangle		(T * dst, uint32_t len); ///< Fills array with Rectangle window
TEM void nyquist		(T * dst, uint32_t len, uint32_t str=1); ///< Fills array with Nyquist window


// Return value from a table with the first half of a dq-symmetric 
// waveform.  The table size must be a power of two.
//
//	'table':	first half of waveform
//	'fbits':	= 31 - (# bits in table)
//	'phase':	phase of lookup (full waveform period is [0, 2^32))
//
//	'phase' bit format (b = 'fbits'):
//	32:			sign bit (0 = positive, 1 = negative)
//	[31, b]:	phase integer part
//	[ b, 0]:	phase fractional part
float atH(const float * src, uint32_t fbits, uint32_t phase);

// Return value from a table with the first quarter of a dbqp-symmetric 
// waveform.  The table size must be a power of two plus one.
//
//	'table':	first quarter of waveform
//	'fbits':	= 30 - (# bits in table)
//	'phase':	phase of lookup (full waveform period is [0, 2^32))
//
//	'phase' bit format (b = 'fbits'):
//	32:			sign bit (0 = positive, 1 = negative)
//	31:			direction bit (0 = forward, 1 = backward)
//	[30, b]:	phase integer part
//	[ b, 0]:	phase fractional part
float atQ(const float * src, uint32_t fbits, uint32_t phase);

// Get interpolation data from integer phasor.
//float getIpol2(uint32_t bits, uint32_t phase, uint32_t &i, uint32_t &ip1);

/// Returns phase increment factor.

///	Multiply by desired frequency in Hz to get integer phase increment.
///
float phaseIncFactor(double framesPerSec);



// Implementation_______________________________________________________________

TEM void cosine(T * dst, uint32_t len){
	double inc = M_2PI / (double)len;
	double phs = inc;
	len >>= 1;
	
	T * dst2 = dst + len;
	
	*dst++  = T( 1);
	*dst2++ = T(-1);

	len -= 1;
	LOOP(len, 1){
		T val = T(cos(phs));
		*dst++  =  val;
		*dst2++ = -val;
		phs += inc;
	}
}

TEM void sine(T * dst, uint32_t len){
	double inc = M_2PI / (double)len;
	double phs = inc;
	len >>= 1;
	
	T * dst2 = dst + len;
	
	*dst++  = T(0);
	*dst2++ = T(0);
	
	--len;
	LOOP(len, 1){
		T val = sin(phs);
		*dst++  =  val;
		*dst2++ = -val;
		phs += inc;
	}
}

// VERY accurate, but not so fast
TEM void sinusoid(T * dst, uint32_t len, double phase, double periods){
	double inc = M_2PI * periods / len;
	for(uint32_t i=0; i<len; ++i){
		*dst++ = sin(inc * i + phase);
	}
}


TEM void multiImpulse(T * dst, uint32_t len, uint32_t hrmLo, uint32_t hrmHi){
	double inc = M_2PI / (double)len;
	uint32_t hLen = len >> 1;
	
	for(uint32_t k = hrmLo; k <= hrmHi; ++k){
		double phaseInc = (double)k * inc;
		double phs = 0.;
		
		T * dst1 = dst;
		
		LOOP(hLen+1, 1){
			*dst1++ += (T)(cos(phs));
			phs += phaseInc;
		}
	}
	
	// Extrapolate remaining from [db] symmetry
	mem::reflectRight(dst + 1, len - 1);
}

TEM void multiSaw(T * dst, uint32_t len, uint32_t hrmLo, uint32_t hrmHi){

	static const double sawFactor = 2.0 / M_PI;
	double inc = M_2PI / (double)len;
	uint32_t hLen = len >> 1;
	
	dst++;
	
	for(uint32_t i = hrmLo; i <= hrmHi; ++i){
		double h = (double)i;
		double phaseInc = h * inc;
		double phs = phaseInc;
		double amp = sawFactor / h;
		
		T * dst1 = dst;
		
		for(uint32_t j=1; j<hLen; ++j){
			*dst1++ += (T)(amp * sin(phs));
			phs += phaseInc;
		}
	}
	
	// Extrapolate remaining from [dp] symmetry
	arr::mirror_dp(dst, len-1);	
}

TEM void multiSquare(T * dst, uint32_t len, uint32_t hrmLo, uint32_t hrmHi){

	static const double sqrFactor = 4.0 / M_PI;
	double inc = M_2PI / (double)len;
	uint32_t qLen = len >> 2;
	
	dst++;
	
	hrmLo |= 1;	// next highest odd if even
	
	// Calculate first quadrant
	for(uint32_t i = hrmLo; i <= hrmHi; i+=2){

		double h = (double)i;
		double phaseInc = h * inc;
		double phs = phaseInc;
		double amp = sqrFactor / h;
		
		T * dst1 = dst;
		
		for(uint32_t j=1; j<=qLen; ++j){
			*dst1++ += (T)(amp * sin(phs));
			phs += phaseInc;
		}
	}

	// Extrapolate remaining from [dbqp] symmetry	
	mem::reflectRight(dst, (len >> 1) - 1);
	arr::mirror_dq(--dst, len);
}


TEM void multiTriangle(T * dst, uint32_t len, uint32_t hrmLo, uint32_t hrmHi){

	static const double triFactor = 8.0 / (M_PI * M_PI);
	double inc = M_2PI / (double)len;
	uint32_t qLen = len >> 2;
	
	dst++;
	
	hrmLo |= 1;	// next highest odd if even
	
	double factor = hrmLo & 0x2 ? -triFactor : triFactor;
	
	// Calculate first quadrant
	for(uint32_t i = hrmLo; i <= hrmHi; i+=2){

		double h = (double)i;
		double phaseInc = h * inc;
		double phs = phaseInc;
		double amp = factor / (h * h);
		factor = -factor;
		
		T * dst1 = dst;
		
		for(uint32_t j=1; j<=qLen; ++j){
			*dst1++ += (T)(amp * sin(phs));
			phs += phaseInc;
		}
	}

	// Extrapolate remaining from [dbqp] symmetry	
	mem::reflectRight(dst, (len >> 1) - 1);
	arr::mirror_dq(--dst, len);
}

/*	1	1
	2	2
	3	4
	5	8
	9	16	
*/
TEM void multiWave(T * dst, uint32_t len, uint32_t order, void (* func)(T *, uint32_t, uint32_t, uint32_t)){

	dst += len * (order - 1);

	func(dst, len, 1, 1);
	
	uint32_t hrmLo = 2;
	uint32_t hrmHi = 2;

	for(uint32_t o=0; o<order-1; o++){
	
		T * dstPrev = dst;
		dst -= len;
		mem::deepCopy(dst, dstPrev, len);
		func(dst, len, hrmLo, hrmHi);
	
		hrmLo = hrmHi + 1;
		hrmHi <<= 1;
	}
}

TEM void window(T * dst, uint32_t len, WindowType type){
	switch(type){
		case BARTLETT:			bartlett(dst, len);			break;
		case BLACKMAN:			blackman(dst, len);			break;
		case BLACKMAN_HARRIS:	blackmanHarris(dst, len);	break;
		case HAMMING:			hamming(dst, len);			break;
		case HANN:				hann(dst, len);				break;
		case WELCH:				welch(dst, len);			break;
		case NYQUIST:			nyquist(dst, len);			break;
		default:				rectangle(dst, len);
	};
}

#define SYM_WIN(period, phs0, eqn) \
	double inc = period / (double)(len);\
	double phs = phs0;\
	T * dst2 = dst + len - 1;\
	*dst++ = (T)eqn;\
	LOOP(len>>1, 1){\
		phs += inc;\
		T val = (T)eqn;\
		*dst++  = val;\
		*dst2-- = val;\
	}
	
TEM void bartlett      (T * dst, uint32_t len){ SYM_WIN(2.   , 0., phs) }
TEM void blackman      (T * dst, uint32_t len){ SYM_WIN(M_2PI, 0., scl::blackman(phs)) }
TEM void blackmanHarris(T * dst, uint32_t len){ SYM_WIN(M_2PI, 0., scl::blackmanHarris(phs)) }
TEM void hamming       (T * dst, uint32_t len){ SYM_WIN(M_2PI, 0., scl::hamming(phs)) }
TEM void hann          (T * dst, uint32_t len){ SYM_WIN(M_2PI, 0., scl::hann(phs)) }
TEM void welch         (T * dst, uint32_t len){ SYM_WIN(2.   ,-1., scl::welch(phs)) }

#undef SYM_WIN

TEM void rectangle(T * dst, uint32_t len){
	for(uint32_t i=0; i<len; ++i) dst[i]=T(1);
}

TEM void nyquist(T * dst, uint32_t len, uint32_t str){
	LOOP(len, str*2){
		dst[(i+0)*str] = T( 1);
		dst[(i+1)*str] = T(-1);
	}
}

inline float atQ(const float * src, uint32_t fbits, uint32_t phase){
	uint32_t sign = phase & MaskSign<float>();
	uint32_t dir = (phase >> 30) & 1; // 0 = fwd or 1 = bwd
	Twiddle<float> v(src[(((phase^-dir) + (dir<<fbits)) & 0x7fffffff) >> fbits]);
	v.i |= sign;	// sign bit
	return v.f;
}

/* inline float tbl::atQ(float * table, uint32_t bits, uint32_t phase){
	switch(phase>>30){
	case 0: return  table[( phase & 0x3fffffff) >> bits]; break;
	case 1: return  table[(-phase & 0x7fffffff) >> bits]; break;
	case 2: return -table[( phase & 0x3fffffff) >> bits]; break;
	default:return -table[(-phase & 0x7fffffff) >> bits]; break;
	}
}*/

inline float atH(const float * src, uint32_t bits, uint32_t phase){
	Twiddle<float> v(src[(phase & 0x7fffffff) >> bits]);
	v.i |= phase & MaskSign<float>();				// sign bit
	return v.f;
}

// i: 0 1 2 3 4 5 6 7
// o: 0 1 2 3 0 1 2 3

//TEM inline T at_dq(const T * src, uint32_t len_2, uint32_t i){
//	return i < len_2 ? src[i] : -src[i - len_2];
//}

inline float phaseIncFactor(double framesPerSec){
	//return float(4294967296. / framesPerSec);
	return float(65536. / framesPerSec) * 65536.;
}

} // tbl::


template <class T>
void addSine(T * dst, uint32_t len, double cycles, double amp, double phs){
	double f = cycles/len;
	for(uint32_t i=0; i<len; ++i){
		double p = (f*i + phs)*M_2PI;
		dst[i] += sin(p) * amp;
	}
}

template <int InvPower, class T>
void addSinesPow(T * dst, uint32_t len, int hNum, int hMul, int hShf, double amp, double phs){
	const double inc1 = M_2PI / len;

	for(int j=0; j<hNum; ++j){
		const int h = j*hMul + hShf;
		if(InvPower && 0==h) continue;
		const double inch = inc1 * h;
		double A = amp;
		
		switch(InvPower){
		case 0: break;
		case 1: A /= h; break;
		case 2: A /= h*h; break;
		case 3: A /= h*h*h; break;
		default:A *= ::pow(h, -InvPower);
		}
		
		for(uint32_t i=0; i<len; ++i){
			dst[i] += A*sin(inch*i + phs);
		}
	}
}

} // gam::

#undef TEM
#undef LOOP

#endif
