//------------------------------------------------------------------------
// Project     : VST SDK
// Version     : 3.6.0
//
// Category    : Interfaces
// Filename    : pluginterfaces/vst/vsttypes.h
// Created by  : Steinberg, 12/2005
// Description : common defines
//
//-----------------------------------------------------------------------------
// LICENSE
// (c) 2013, Steinberg Media Technologies GmbH, All Rights Reserved
//-----------------------------------------------------------------------------
// This Software Development Kit may not be distributed in parts or its entirety
// without prior written agreement by Steinberg Media Technologies GmbH.
// This SDK must not be used to re-engineer or manipulate any technology used
// in any Steinberg or Third-party application or software module,
// unless permitted by law.
// Neither the name of the Steinberg Media Technologies nor the names of its
// contributors may be used to endorse or promote products derived from this
// software without specific prior written permission.
//
// THIS SDK IS PROVIDED BY STEINBERG MEDIA TECHNOLOGIES GMBH "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL STEINBERG MEDIA TECHNOLOGIES GMBH BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//----------------------------------------------------------------------------------

#ifndef __vsttypes__
#define __vsttypes__

#include "pluginterfaces/base/fstrdefs.h"

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {
//------------------------------------------------------------------------
#ifndef kVstVersionString
#define kVstVersionString	"VST 3.6.0"	///< SDK version for PClassInfo2
#endif

#define kVstVersionMajor	3
#define kVstVersionMinor	5
#define kVstVersionSub		2

// this allows to write things like: #if VST_VERSION >= 0x030500 // note that 3.10.0 is 0x030a00
#define VST_VERSION ((kVstVersionMajor << 16) | (kVstVersionMinor << 8) | kVstVersionSub)

//------------------------------------------------------------------------
// struct alignment definitions
//------------------------------------------------------------------------
#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
	#define VST3_STRUCT	struct __attribute__((aligned(4)))
#elif WINDOWS
	#if PLATFORM_64
		#define VST3_PACKED_SIZE	8
	#else
		#define VST3_PACKED_SIZE	4
	#endif
	#define VST3_STRUCT	__declspec(align(VST3_PACKED_SIZE)) struct
#else
	#error "unsupported compiler"
#endif

//------------------------------------------------------------------------
/** \defgroup vst3typedef VST 3 Data Types
*/
/*@{*/
//------------------------------------------------------------------------
// string types
//------------------------------------------------------------------------
typedef char16 TChar;			///< UTF-16 character
typedef TChar String128[128];	///< 128 character UTF-16 string
typedef const char8* CString;	///< C-String

//------------------------------------------------------------------------
// general
//------------------------------------------------------------------------
typedef int32 MediaType;		///< media type (audio/event)
typedef int32 BusDirection;		///< bus direction (in/out)
typedef int32 BusType;			///< bus type (main/aux)
typedef int32 IoMode;			///< I/O mode
typedef int32 UnitID;			///< unit identifier
typedef double ParamValue;		///< parameter value type
typedef uint32 ParamID;			///< parameter identifier
typedef int32 ProgramListID;	///< program list identifier
typedef int16 CtrlNumber;		///< MIDI controller number (see \ref ControllerNumbers for allowed values)

typedef double TQuarterNotes;	///< time expressed in quarter notes
typedef int64 TSamples;			///< time expressed in audio samples

//------------------------------------------------------------------------
// Audio Types
//------------------------------------------------------------------------
typedef float Sample32;				///< 32-bit precision audio sample
typedef double Sample64;			///< 64-bit precision audio sample
typedef double SampleRate;			///< sample rate
/*@}*/

//------------------------------------------------------------------------
/** \defgroup speakerArrangements Speaker Arrangements
\image html "vst3_speaker_types.jpg"
\n
A SpeakerArrangement is a bitset combination of speakers. For example:
\code
const SpeakerArrangement kStereo = kSpeakerL | kSpeakerR; // => hex: 0x03 / binary: 0011.
\endcode*/
//------------------------------------------------------------------------
/**@{*/
typedef uint64 SpeakerArrangement;	///< Bitset of speakers
typedef uint64 Speaker;	            ///< Bit for one speaker
/**@}*/

//------------------------------------------------------------------------
/** Speaker Definitions.
\ingroup speakerArrangements */
//------------------------------------------------------------------------
/**@{*/
const Speaker kSpeakerL    = 1 << 0;		///< Left (L)
const Speaker kSpeakerR    = 1 << 1;		///< Right (R)
const Speaker kSpeakerC    = 1 << 2;		///< Center (C)
const Speaker kSpeakerLfe  = 1 << 3;		///< Subbass (Lfe)
const Speaker kSpeakerLs   = 1 << 4;		///< Left Surround (Ls)
const Speaker kSpeakerRs   = 1 << 5;		///< Right Surround (Rs)
const Speaker kSpeakerLc   = 1 << 6;		///< Left of Center (Lc)
const Speaker kSpeakerRc   = 1 << 7;		///< Right of Center (Rc)
const Speaker kSpeakerS    = 1 << 8;		///< Surround (S)
const Speaker kSpeakerCs   = kSpeakerS;		///< Center of Surround (Cs) = Surround (S)
const Speaker kSpeakerSl   = 1 << 9;		///< Side Left (Sl)
const Speaker kSpeakerSr   = 1 << 10;		///< Side Right (Sr)
const Speaker kSpeakerTm   = 1 << 11;		///< Top Middle - Center Over-head (Tm)
const Speaker kSpeakerTfl  = 1 << 12;		///< Top Front Left (Tfl)
const Speaker kSpeakerTfc  = 1 << 13;		///< Top Front Center (Tfc)
const Speaker kSpeakerTfr  = 1 << 14;		///< Top Front Right (Tfr)
const Speaker kSpeakerTrl  = 1 << 15;		///< Top Rear Left (Trl)
const Speaker kSpeakerTrc  = 1 << 16;		///< Top Rear Center (Trc)
const Speaker kSpeakerTrr  = 1 << 17;		///< Top Rear Right (Trr)
const Speaker kSpeakerLfe2 = 1 << 18;		///< Subbass 2 (Lfe2)
const Speaker kSpeakerM    = 1 << 19;		///< Mono (M)

const Speaker kSpeakerW	 = 1 << 20;			///< B-Format W
const Speaker kSpeakerX	 = 1 << 21;			///< B-Format X
const Speaker kSpeakerY	 = 1 << 22;			///< B-Format Y
const Speaker kSpeakerZ	 = 1 << 23;			///< B-Format Z

const Speaker kSpeakerTsl = 1 << 24;		///< Top Side Left (Tsl)
const Speaker kSpeakerTsr = 1 << 25;		///< Top Side Right (Tsr)
const Speaker kSpeakerLcs = 1 << 26;		///< Left of Center Surround (Lcs)
const Speaker kSpeakerRcs = 1 << 27;		///< Right of Center Surround (Rcs)

const Speaker kSpeakerBfl = 1 << 28;		///< Bottom Front Left (Bfl)
const Speaker kSpeakerBfc = 1 << 29;		///< Bottom Front Center (Bfc)
const Speaker kSpeakerBfr = 1 << 30;		///< Bottom Front Right (Bfr)


//------------------------------------------------------------------------
/** @}*/

//------------------------------------------------------------------------
/** Speaker Arrangement Definitions (SpeakerArrangement)*/
//------------------------------------------------------------------------
namespace SpeakerArr
{
//------------------------------------------------------------------------
/** Speaker Arrangement Definitions.
\ingroup speakerArrangements */
/*@{*/
const SpeakerArrangement kEmpty			 = 0;          ///< empty arrangement
const SpeakerArrangement kMono			 = kSpeakerM;  ///< M
const SpeakerArrangement kStereo		 = kSpeakerL  | kSpeakerR;    ///< L R
const SpeakerArrangement kStereoSurround = kSpeakerLs | kSpeakerRs;	  ///< Ls Rs
const SpeakerArrangement kStereoCenter	 = kSpeakerLc | kSpeakerRc;   ///< Lc Rc
const SpeakerArrangement kStereoSide	 = kSpeakerSl | kSpeakerSr;	  ///< Sl Sr
const SpeakerArrangement kStereoCLfe	 = kSpeakerC  | kSpeakerLfe;  ///< C Lfe
/** L R C */
const SpeakerArrangement k30Cine		 = kSpeakerL  | kSpeakerR | kSpeakerC;
/** L R S */
const SpeakerArrangement k30Music		 = kSpeakerL  | kSpeakerR | kSpeakerS;
/** L R C   Lfe */
const SpeakerArrangement k31Cine		 = kSpeakerL  | kSpeakerR | kSpeakerC   | kSpeakerLfe;
/** L R Lfe S */
const SpeakerArrangement k31Music		 = kSpeakerL  | kSpeakerR | kSpeakerLfe | kSpeakerS;
/** L R C   S (LCRS) */
const SpeakerArrangement k40Cine		 = kSpeakerL  | kSpeakerR | kSpeakerC   | kSpeakerS;
/** L R Ls  Rs (Quadro) */
const SpeakerArrangement k40Music		 = kSpeakerL  | kSpeakerR | kSpeakerLs  | kSpeakerRs;
/** L R C   Lfe S (LCRS+Lfe) */
const SpeakerArrangement k41Cine		 = kSpeakerL  | kSpeakerR | kSpeakerC   | kSpeakerLfe | kSpeakerS;
/** L R Lfe Ls Rs (Quadro+Lfe) */
const SpeakerArrangement k41Music		 = kSpeakerL  | kSpeakerR | kSpeakerLfe | kSpeakerLs  | kSpeakerRs;
/** L R C   Ls Rs */
const SpeakerArrangement k50			 = kSpeakerL  | kSpeakerR | kSpeakerC   | kSpeakerLs  | kSpeakerRs;
/** L R C  Lfe Ls Rs */
const SpeakerArrangement k51			 = kSpeakerL  | kSpeakerR | kSpeakerC   | kSpeakerLfe | kSpeakerLs | kSpeakerRs;
/** L R C  Ls  Rs Cs */
const SpeakerArrangement k60Cine		 = kSpeakerL  | kSpeakerR | kSpeakerC   | kSpeakerLs  | kSpeakerRs | kSpeakerCs;
/** L R Ls Rs  Sl Sr */
const SpeakerArrangement k60Music		 = kSpeakerL  | kSpeakerR | kSpeakerLs  | kSpeakerRs  | kSpeakerSl | kSpeakerSr;
/** L R C  Lfe Ls Rs Cs */
const SpeakerArrangement k61Cine		 = kSpeakerL  | kSpeakerR | kSpeakerC   | kSpeakerLfe | kSpeakerLs | kSpeakerRs | kSpeakerCs;
/** L R Lfe Ls  Rs Sl Sr */
const SpeakerArrangement k61Music		 = kSpeakerL  | kSpeakerR | kSpeakerLfe | kSpeakerLs  | kSpeakerRs | kSpeakerSl | kSpeakerSr;
/** L R C   Ls  Rs Lc Rc */
const SpeakerArrangement k70Cine		 = kSpeakerL  | kSpeakerR | kSpeakerC   | kSpeakerLs  | kSpeakerRs | kSpeakerLc | kSpeakerRc;
/** L R C   Ls  Rs Sl Sr */
const SpeakerArrangement k70Music		 = kSpeakerL  | kSpeakerR | kSpeakerC   | kSpeakerLs  | kSpeakerRs | kSpeakerSl | kSpeakerSr;
/** L R C Lfe Ls Rs Lc Rc */
const SpeakerArrangement k71Cine		 = kSpeakerL  | kSpeakerR | kSpeakerC   | kSpeakerLfe | kSpeakerLs | kSpeakerRs | kSpeakerLc | kSpeakerRc;
const SpeakerArrangement k71CineFullFront= k71Cine;
/** L R C Lfe Ls Rs Lcs Rcs */
const SpeakerArrangement k71CineFullRear = kSpeakerL  | kSpeakerR | kSpeakerC   | kSpeakerLfe | kSpeakerLs | kSpeakerRs | kSpeakerLcs | kSpeakerRcs;
/** L R C Lfe Ls Rs Sl Sr */
const SpeakerArrangement k71Music		 = kSpeakerL  | kSpeakerR | kSpeakerC   | kSpeakerLfe | kSpeakerLs | kSpeakerRs | kSpeakerSl | kSpeakerSr;
const SpeakerArrangement k71CineSideFill = k71Music;
/** L R C Ls  Rs Lc Rc Cs */
const SpeakerArrangement k80Cine		 = kSpeakerL  | kSpeakerR | kSpeakerC   | kSpeakerLs  | kSpeakerRs | kSpeakerLc | kSpeakerRc | kSpeakerCs;
/** L R C Ls  Rs Cs Sl Sr */
const SpeakerArrangement k80Music		 = kSpeakerL  | kSpeakerR | kSpeakerC   | kSpeakerLs  | kSpeakerRs | kSpeakerCs | kSpeakerSl | kSpeakerSr;
/** L R C Lfe Ls Rs Lc Rc Cs */
const SpeakerArrangement k81Cine		 = kSpeakerL  | kSpeakerR | kSpeakerC   | kSpeakerLfe | kSpeakerLs | kSpeakerRs | kSpeakerLc | kSpeakerRc | kSpeakerCs;
/** L R C Lfe Ls Rs Cs Sl Sr */
const SpeakerArrangement k81Music		 = kSpeakerL  | kSpeakerR | kSpeakerC   | kSpeakerLfe | kSpeakerLs | kSpeakerRs | kSpeakerCs | kSpeakerSl | kSpeakerSr;

/** W X Y Z (First Order) */
const SpeakerArrangement kBFormat1stOrder = kSpeakerW | kSpeakerX | kSpeakerY   | kSpeakerZ;
const SpeakerArrangement kBFormat		 = kBFormat1stOrder;

/*-----------*/
/* 3D formats */
/*-----------*/
/** L R Ls Rs Tfl Tfr Trl Trr */
const SpeakerArrangement k80Cube		   = kSpeakerL | kSpeakerR | kSpeakerLs | kSpeakerRs  | kSpeakerTfl| kSpeakerTfr| kSpeakerTrl | kSpeakerTrr;
/** L R C Lfe Ls Rs Cs Tm */
const SpeakerArrangement k71CineTopCenter  = kSpeakerL | kSpeakerR | kSpeakerC  | kSpeakerLfe | kSpeakerLs | kSpeakerRs | kSpeakerCs  | kSpeakerTm; 
/** L R C Lfe Ls Rs Cs Tfc */
const SpeakerArrangement k71CineCenterHigh = kSpeakerL | kSpeakerR | kSpeakerC  | kSpeakerLfe | kSpeakerLs | kSpeakerRs | kSpeakerCs  | kSpeakerTfc; 
/** L R C Lfe Ls Rs Tfl Tfr */
const SpeakerArrangement k71CineFrontHigh  = kSpeakerL | kSpeakerR | kSpeakerC  | kSpeakerLfe | kSpeakerLs | kSpeakerRs | kSpeakerTfl | kSpeakerTfr; 
const SpeakerArrangement k71MPEG3D		   = k71CineFrontHigh;
/** L R C Lfe Ls Rs Tsl Tsr */
const SpeakerArrangement k71CineSideHigh   = kSpeakerL | kSpeakerR | kSpeakerC  | kSpeakerLfe | kSpeakerLs | kSpeakerRs | kSpeakerTsl | kSpeakerTsr; 

/** L R Lfe Ls Rs Tfl Tfc Tfr Bfc */
const SpeakerArrangement k81MPEG3D		  = kSpeakerL | kSpeakerR | kSpeakerLfe | kSpeakerLs | kSpeakerRs |
										    kSpeakerTfl | kSpeakerTfc | kSpeakerTfr | kSpeakerBfc;

/** L R C Ls Rs Tfl Tfr Trl Trr */
const SpeakerArrangement k90			 = kSpeakerL  | kSpeakerR | kSpeakerC | kSpeakerLs  | kSpeakerRs |
										   kSpeakerTfl| kSpeakerTfr | kSpeakerTrl | kSpeakerTrr;			
/** L R C Lfe Ls Rs Tfl Tfr Trl Trr */
const SpeakerArrangement k91			 = kSpeakerL  | kSpeakerR | kSpeakerC | kSpeakerLfe | kSpeakerLs | kSpeakerRs  |
										   kSpeakerTfl| kSpeakerTfr | kSpeakerTrl | kSpeakerTrr;
/** L R C Ls Rs Tm Tfl Tfr Trl Trr */
const SpeakerArrangement k100			 = kSpeakerL  | kSpeakerR | kSpeakerC | kSpeakerLs  | kSpeakerRs | 
										   kSpeakerTm | kSpeakerTfl | kSpeakerTfr | kSpeakerTrl | kSpeakerTrr;
/** L R C Lfe Ls Rs Tm Tfl Tfr Trl Trr */
const SpeakerArrangement k101			 = kSpeakerL  | kSpeakerR | kSpeakerC | kSpeakerLfe | kSpeakerLs | kSpeakerRs  | 
										   kSpeakerTm | kSpeakerTfl | kSpeakerTfr | kSpeakerTrl | kSpeakerTrr;
const SpeakerArrangement k101MPEG3D		 = k101;

/** L R C Lfe Ls Rs Tfl Tfc Tfr Trl Trr Lfe2 */
const SpeakerArrangement k102			 = kSpeakerL  | kSpeakerR | kSpeakerC  | kSpeakerLfe | kSpeakerLs | kSpeakerRs  |
										   kSpeakerTfl| kSpeakerTfc | kSpeakerTfr | kSpeakerTrl | kSpeakerTrr | kSpeakerLfe2;

/** L R C Ls Rs Tm Tfl Tfc Tfr Trl Trr */
const SpeakerArrangement k110			 = kSpeakerL  | kSpeakerR | kSpeakerC | kSpeakerLs  | kSpeakerRs |
										   kSpeakerTm | kSpeakerTfl | kSpeakerTfc | kSpeakerTfr | kSpeakerTrl | kSpeakerTrr;
/** L R C Lfe Ls Rs Tm Tfl Tfc Tfr Trl Trr */
const SpeakerArrangement k111			 = kSpeakerL  | kSpeakerR | kSpeakerC | kSpeakerLfe | kSpeakerLs | kSpeakerRs | 
										   kSpeakerTm | kSpeakerTfl | kSpeakerTfc | kSpeakerTfr | kSpeakerTrl | kSpeakerTrr;
/** L R C Lfe Ls Rs Lc Rc Tfl Tfc Tfr Trl Trr Lfe2 */
const SpeakerArrangement k122			 = kSpeakerL  | kSpeakerR | kSpeakerC  | kSpeakerLfe | kSpeakerLs | kSpeakerRs	| kSpeakerLc  | kSpeakerRc |
										   kSpeakerTfl| kSpeakerTfc | kSpeakerTfr | kSpeakerTrl | kSpeakerTrr | kSpeakerLfe2;
/** L R C Ls Rs Cs Tm Tfl Tfc Tfr Trl Trc Trr */
const SpeakerArrangement k130			 = kSpeakerL  | kSpeakerR | kSpeakerC | kSpeakerLs  | kSpeakerRs | kSpeakerCs |
										   kSpeakerTm | kSpeakerTfl | kSpeakerTfc | kSpeakerTfr | kSpeakerTrl | kSpeakerTrc | kSpeakerTrr;
/** L R C Lfe Ls Rs Cs Tm Tfl Tfc Tfr Trl Trc Trr */
const SpeakerArrangement k131			 = kSpeakerL  | kSpeakerR | kSpeakerC | kSpeakerLfe | kSpeakerLs | kSpeakerRs | kSpeakerCs |
										   kSpeakerTm | kSpeakerTfl | kSpeakerTfc | kSpeakerTfr | kSpeakerTrl | kSpeakerTrc | kSpeakerTrr;

/** L R C Lfe Ls Rs Lc Rc Cs Sl Sr Tm Tfl Tfc Tfr Trl Trc Trr Lfe2 Tsl Tsr Bfl Blc Bfr */
const SpeakerArrangement k222			= kSpeakerL  | kSpeakerR | kSpeakerC  | kSpeakerLfe | kSpeakerLs | kSpeakerRs | kSpeakerLc | kSpeakerRc | kSpeakerCs | kSpeakerSl | kSpeakerSr | 
										   kSpeakerTm | kSpeakerTfl | kSpeakerTfc | kSpeakerTfr | kSpeakerTrl | kSpeakerTrc | kSpeakerTrr | kSpeakerLfe2 | kSpeakerTsl | kSpeakerTsr | 
										   kSpeakerBfl| kSpeakerBfc | kSpeakerBfr;

//------------------------------------------------------------------------
/** Speaker Arrangement String Representation.
\ingroup speakerArrangements */
/*@{*/
const CString kStringEmpty		= "";
const CString kStringMono		= "Mono";
const CString kStringStereo		= "Stereo";
const CString kStringStereoR	= "Stereo (Ls Rs)";
const CString kStringStereoC	= "Stereo (Lc Rc)";
const CString kStringStereoSide	= "Stereo (Sl Sr)";
const CString kStringStereoCLfe	= "Stereo (C LFE)";

const CString kString30Cine		= "LRC";
const CString kString30Music	= "LRS";
const CString kString31Cine		= "LRC+LFE";
const CString kString31Music	= "LRS+LFE";
const CString kString40Cine		= "LRCS";
const CString kString40Music	= "Quadro";
const CString kString41Cine		= "LRCS+LFE";
const CString kString41Music	= "Quadro+LFE";
const CString kString50			= "5.0";
const CString kString51			= "5.1";
const CString kString60Cine		= "6.0 Cine";
const CString kString60Music	= "6.0 Music";
const CString kString61Cine		= "6.1 Cine";
const CString kString61Music	= "6.1 Music";
const CString kString70Cine		= "7.0 Cine";
const CString kString70Music	= "7.0 Music";
const CString kString71Cine		= "7.1 Cine";
const CString kString71Music	= "7.1 Music";
const CString kString71CineTopCenter	= "7.1 Cine Top Center";
const CString kString71CineCenterHigh	= "7.1 Cine Center High";
const CString kString71CineFrontHigh	= "7.1 Cine Front High";
const CString kString71CineSideHigh		= "7.1 Cine Side High";
const CString kString71CineFullRear		= "7.1 Cine Full Rear";
const CString kString80Cine		= "8.0 Cine";
const CString kString80Music	= "8.0 Music";
const CString kString80Cube		= "8.0 Cube";
const CString kString81Cine		= "8.1 Cine";
const CString kString81Music	= "8.1 Music";
const CString kString102		= "10.2";
const CString kString122		= "12.2";
const CString kString90			= "9.0";
const CString kString91			= "9.1";
const CString kString100		= "10.0";
const CString kString101		= "10.1";
const CString kString110		= "11.0";
const CString kString111		= "11.1";
const CString kString130		= "13.0";
const CString kString131		= "13.1";
const CString kString222		= "22.2";
const CString kStringBFormat1stOrder = "BFormat";
/*@}*/

//------------------------------------------------------------------------
/** Speaker Arrangement String Representation with Speakers Name.
\ingroup speakerArrangements */
/*@{*/
const CString kStringMonoS		= "M";
const CString kStringStereoS	= "L R";
const CString kStringStereoRS	= "Ls Rs";
const CString kStringStereoCS	= "Lc Rc";
const CString kStringStereoSS	= "Sl Sr";
const CString kStringStereoCLfeS= "C LFE";
const CString kString30CineS	= "L R C";
const CString kString30MusicS	= "L R S";
const CString kString31CineS	= "L R C LFE";
const CString kString31MusicS	= "L R LFE S";
const CString kString40CineS	= "L R C S";
const CString kString40MusicS	= "L R Ls Rs";
const CString kString41CineS	= "L R C LFE S";
const CString kString41MusicS	= "L R LFE Ls Rs";
const CString kString50S		= "L R C Ls Rs";
const CString kString51S		= "L R C LFE Ls Rs";
const CString kString60CineS	= "L R C Ls Rs Cs";
const CString kString60MusicS	= "L R Ls Rs Sl Sr";
const CString kString61CineS	= "L R C LFE Ls Rs Cs";
const CString kString61MusicS	= "L R LFE Ls Rs Sl Sr";
const CString kString70CineS	= "L R C Ls Rs Lc Rc";
const CString kString70MusicS	= "L R C Ls Rs Sl Sr";
const CString kString71CineS	= "L R C LFE Ls Rs Lc Rc";
const CString kString71MusicS	= "L R C LFE Ls Rs Sl Sr";
const CString kString80CineS	= "L R C Ls Rs Lc Rc Cs";
const CString kString80MusicS	= "L R C Ls Rs Cs Sl Sr";
const CString kString81CineS	= "L R C LFE Ls Rs Lc Rc Cs";
const CString kString81MusicS	= "L R C LFE Ls Rs Cs Sl Sr";
const CString kString80CubeS	= "L R Ls Rs Tfl Tfr Trl Trr";
const CString kStringBFormat1stOrderS	= "W X Y Z";
const CString kString71CineTopCenterS	= "L R C LFE Ls Rs Cs Tm";
const CString kString71CineCenterHighS	= "L R C LFE Ls Rs Cs Tfc";
const CString kString71CineFrontHighS	= "L R C LFE Ls Rs Tfl Tfl";
const CString kString71CineSideHighS	= "L R C LFE Ls Rs Tsl Tsl";
const CString kString71CineFullRearS	= "L R C LFE Ls Rs Lcs Rcs";
const CString kString90S		= "L R C Ls Rs Tfl Tfr Trl Trr";
const CString kString91S		= "L R C LFE Ls Rs Tfl Tfr Trl Trr";
const CString kString100S		= "L R C Ls Rs Tm Tfl Tfr Trl Trr";
const CString kString101S		= "L R C LFE Ls Rs Tm Tfl Tfr Trl Trr";
const CString kString110S		= "L R C Ls Rs Tm Tfl Tfc Tfr Trl Trr";
const CString kString111S		= "L R C LFE Ls Rs Tm Tfl Tfc Tfr Trl Trr";
const CString kString130S		= "L R C Ls Rs Cs Tm Tfl Tfc Tfr Trl Trc Trr";
const CString kString131S		= "L R C LFE Ls Rs Cs Tm Tfl Tfc Tfr Trl Trc Trr";
const CString kString102S		= "L R C LFE Ls Rs Tfl Tfc Tfr Trl Trr LFE2";
const CString kString122S		= "L R C LFE Ls Rs Lc Rc Tfl Tfc Tfr Trl Trr LFE2";
const CString kString81MPEGS	= "L R LFE Ls Rs Tfl Tfc Tfr Bfc";
const CString kString101MPEGS	= "L R C LFE Ls Rs Tm Tfl Tfr Trl Trr";
const CString kString222S		= "L R C LFE Ls Rs Lc Rc Cs Sl Sr Tm Tfl Tfc Tfr Trl Trc Trr LFE2 Tsl Tsr Bfl Blc Bfr";

/*@}*/

//------------------------------------------------------------------------
/** Returns number of channels used in speaker arrangement.
\ingroup speakerArrangements */
/*@{*/
inline int32 getChannelCount (SpeakerArrangement arr)
{
	int32 count = 0;
	while (arr)
	{
		if (arr & (SpeakerArrangement)1)
			++count;
		arr >>= 1;
	}
	return count;
}

//------------------------------------------------------------------------
/** Returns the index of a given speaker in a speaker arrangement (-1 if speaker is not part of the arrangement). */
inline int32 getSpeakerIndex (Speaker speaker, SpeakerArrangement arrangement)
{
	// check if speaker is present in arrangement
	if ((arrangement & speaker) == 0)
		return -1;

	int32 result = 0;
	Speaker i = 1;
	while (i < speaker)
	{
		if (arrangement & i)
			result++;
		i <<= 1;
	}

	return result;
}

//------------------------------------------------------------------------
/** Returns the speaker arrangement associated to a string representation.
	Returns kEmpty if no associated arrangement is known.
*/
inline SpeakerArrangement getSpeakerArrangementFromString (CString arrStr)
{
	if (!strcmp8 (arrStr, kStringMono))
		return kMono;
	if (!strcmp8 (arrStr, kStringStereo))
		return kStereo;
	if (!strcmp8 (arrStr, kStringStereoR))
		return kStereoSurround;
	if (!strcmp8 (arrStr, kStringStereoC))
		return kStereoCenter;
	if (!strcmp8 (arrStr, kStringStereoSide))
		return kStereoSide;
	if (!strcmp8 (arrStr, kStringStereoCLfe))
		return kStereoCLfe;
	if (!strcmp8 (arrStr, kString30Cine))
		return k30Cine;
	if (!strcmp8 (arrStr, kString30Music))
		return k30Music;
	if (!strcmp8 (arrStr, kString31Cine))
		return k31Cine;
	if (!strcmp8 (arrStr, kString31Music))
		return k31Music;
	if (!strcmp8 (arrStr, kString40Cine))
		return k40Cine;
	if (!strcmp8 (arrStr, kString40Music))
		return k40Music;
	if (!strcmp8 (arrStr, kString41Cine))
		return k41Cine;
	if (!strcmp8 (arrStr, kString41Music))
		return k41Music;
	if (!strcmp8 (arrStr, kString50))
		return k50;
	if (!strcmp8 (arrStr, kString51))
		return k51;
	if (!strcmp8 (arrStr, kString60Cine))
		return k60Cine;
	if (!strcmp8 (arrStr, kString60Music))
		return k60Music;
	if (!strcmp8 (arrStr, kString61Cine))
		return k61Cine;
	if (!strcmp8 (arrStr, kString61Music))
		return k61Music;
	if (!strcmp8 (arrStr, kString70Cine))
		return k70Cine;
	if (!strcmp8 (arrStr, kString70Music))
		return k70Music;
	if (!strcmp8 (arrStr, kString71Cine))
		return k71Cine;
	if (!strcmp8 (arrStr, kString71Music))
		return k71Music;
	if (!strcmp8 (arrStr, kString80Cine))
		return k80Cine;
	if (!strcmp8 (arrStr, kString80Music))
		return k80Music;
	if (!strcmp8 (arrStr, kString81Cine))
		return k81Cine;
	if (!strcmp8 (arrStr, kString81Music))
		return k81Music;
	if (!strcmp8 (arrStr, kString102))
		return k102;
	if (!strcmp8 (arrStr, kString122))
		return k122;
	if (!strcmp8 (arrStr, kString80Cube))
		return k80Cube;
	if (!strcmp8 (arrStr, kStringBFormat1stOrder))
		return kBFormat1stOrder;
	if (!strcmp8 (arrStr, kString71CineTopCenter))
		return k71CineTopCenter;
	if (!strcmp8 (arrStr, kString71CineCenterHigh))
		return k71CineCenterHigh;
	if (!strcmp8 (arrStr, kString71CineFrontHigh))
		return k71CineFrontHigh;
	if (!strcmp8 (arrStr, kString71CineSideHigh))
		return k71CineSideHigh;
	if (!strcmp8 (arrStr, kString71CineFullRear))
		return k71CineFullRear;
	if (!strcmp8 (arrStr, kString90))
		return k90;
	if (!strcmp8 (arrStr, kString91))
		return k91;
	if (!strcmp8 (arrStr, kString100))
		return k100;
	if (!strcmp8 (arrStr, kString101))
		return k101;
	if (!strcmp8 (arrStr, kString110))
		return k110;
	if (!strcmp8 (arrStr, kString111))
		return k111;
	if (!strcmp8 (arrStr, kString130))
		return k130;
	if (!strcmp8 (arrStr, kString131))
		return k131;
	if (!strcmp8 (arrStr, kString222))
		return k222;
	return kEmpty;
}

//------------------------------------------------------------------------
/** Returns the string representation of a given speaker arrangement.
	Returns kStringEmpty if arr is unknown.
*/
inline CString getSpeakerArrangementString (SpeakerArrangement arr, bool withSpeakersName)
{
	switch (arr)
	{
		case kMono:				return withSpeakersName ? kStringMonoS		: kStringMono;
		case kStereo:			return withSpeakersName ? kStringStereoS	: kStringStereo;
		case kStereoSurround:	return withSpeakersName ? kStringStereoRS	: kStringStereoR;
		case kStereoCenter:		return withSpeakersName ? kStringStereoCS	: kStringStereoC;
		case kStereoSide:		return withSpeakersName ? kStringStereoSS	: kStringStereoSide;
		case kStereoCLfe:		return withSpeakersName ? kStringStereoCLfeS: kStringStereoCLfe;
		case k30Cine:			return withSpeakersName ? kString30CineS	: kString30Cine;
		case k30Music:			return withSpeakersName ? kString30MusicS	: kString30Music;
		case k31Cine:			return withSpeakersName ? kString31CineS	: kString31Cine;
		case k31Music:			return withSpeakersName ? kString31MusicS	: kString31Music;
		case k40Cine:			return withSpeakersName ? kString40CineS	: kString40Cine;
		case k40Music:			return withSpeakersName ? kString40MusicS	: kString40Music;
		case k41Cine:			return withSpeakersName ? kString41CineS	: kString41Cine;
		case k41Music:			return withSpeakersName ? kString41MusicS	: kString41Music;
		case k50:				return withSpeakersName ? kString50S		: kString50;
		case k51:				return withSpeakersName ? kString51S		: kString51;
		case k60Cine:			return withSpeakersName ? kString60CineS	: kString60Cine;
		case k60Music:			return withSpeakersName ? kString60MusicS	: kString60Music;
		case k61Cine:			return withSpeakersName ? kString61CineS	: kString61Cine;
		case k61Music:			return withSpeakersName ? kString61MusicS	: kString61Music;
		case k70Cine:			return withSpeakersName ? kString70CineS	: kString70Cine;
		case k70Music:			return withSpeakersName ? kString70MusicS	: kString70Music;
		case k71Cine:			return withSpeakersName ? kString71CineS	: kString71Cine;
		case k71Music:			return withSpeakersName ? kString71MusicS	: kString71Music;
		case k80Cine:			return withSpeakersName ? kString80CineS	: kString80Cine;
		case k80Music:			return withSpeakersName ? kString80MusicS	: kString80Music;
		case k81Cine:			return withSpeakersName ? kString81CineS	: kString81Cine;
		case k81Music:			return withSpeakersName ? kString81MusicS	: kString81Music;
		case k102:				return withSpeakersName ? kString102S		: kString102;
		case k122:				return withSpeakersName ? kString122S		: kString122;
		case k80Cube:			return withSpeakersName ? kString80CubeS	: kString80Cube;
		case kBFormat1stOrder:	return withSpeakersName ? kStringBFormat1stOrderS	: kStringBFormat1stOrder;
		case k71CineTopCenter:	return withSpeakersName ? kString71CineTopCenterS	: kString71CineTopCenter;
		case k71CineCenterHigh:	return withSpeakersName ? kString71CineCenterHighS	: kString71CineCenterHigh;
		case k71CineFrontHigh:	return withSpeakersName ? kString71CineFrontHighS	: kString71CineFrontHigh;
		case k71CineSideHigh:	return withSpeakersName ? kString71CineSideHighS	: kString71CineSideHigh;
		case k71CineFullRear:	return withSpeakersName ? kString71CineFullRearS	: kString71CineFullRear;
		case k90:				return withSpeakersName ? kString90S		: kString90;
		case k91:				return withSpeakersName ? kString91S		: kString91;
		case k100:				return withSpeakersName ? kString100S		: kString100;
		case k101:				return withSpeakersName ? kString101S		: kString101;
		case k110:				return withSpeakersName ? kString110S		: kString110;
		case k111:				return withSpeakersName ? kString111S		: kString111;
		case k130:				return withSpeakersName ? kString130S		: kString130;
		case k131:				return withSpeakersName ? kString131S		: kString131;
		case k222:				return withSpeakersName ? kString222S		: kString222;
			break;
	}
	return kStringEmpty;
}

//------------------------------------------------------------------------
/** Returns a CString representation of a given speaker in a given arrangement
*/
inline CString getSpeakerShortName (const SpeakerArrangement& arr, int32 index)
{
	SpeakerArrangement arrTmp = arr;

	bool found = false;
	int32 index2 = -1;
	int32 pos = -1;
	while (arrTmp)
	{
		if (arrTmp & 0x1)
			index2++;
		pos++;
		if (index2 == index)
		{
			found = true;
			break;
		}
		arrTmp = arrTmp >> 1;
	}

	if (!found)
		return "";

	switch (1 << pos)
	{
		case kSpeakerL:
			return "L";
		case kSpeakerR:
			return "R";
		case kSpeakerC:
			return "C";
		case kSpeakerLfe:
			return "LFE";
		case kSpeakerLs:
			return "Ls";
		case kSpeakerRs:
			return "Rs";
		case kSpeakerLc:
			return "Lc";
		case kSpeakerRc:
			return "Rc";
		case kSpeakerS:
			return "S";
		case kSpeakerSl:
			return "Sl";
		case kSpeakerSr:
			return "Sr";
		case kSpeakerTm:
			return "Tm";
		case kSpeakerTfl:
			return "Tfl";
		case kSpeakerTfc:
			return "Tfc";
		case kSpeakerTfr:
			return "Tfr";
		case kSpeakerTrl:
			return "Trl";
		case kSpeakerTrc:
			return "Trc";
		case kSpeakerTrr:
			return "Trr";
		case kSpeakerLfe2:
			return "LFE2";
		case kSpeakerM:
			return "M";

		case kSpeakerW:
			return "W";
		case kSpeakerX:
			return "X";
		case kSpeakerY:
			return "Y";
		case kSpeakerZ:
			return "Z";

		case kSpeakerTsl:
			return "Tsl";
		case kSpeakerTsr:
			return "Tsr";
		case kSpeakerLcs:
			return "Lcs";
		case kSpeakerRcs:
			return "Rcs";

		case kSpeakerBfl:
			return "Bfl";
		case kSpeakerBfc:
			return "Bfc";
		case kSpeakerBfr:
			return "Bfr";
	}
	return "";
}

/*@}*/

//------------------------------------------------------------------------
} // namespace SpeakerArr

} // namespace Vst
} // namespace Steinberg


#endif	// __vsttypes__