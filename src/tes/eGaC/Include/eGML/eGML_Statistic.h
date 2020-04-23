/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_Statistic.h
* %version:        134.0 %
* 
* author:          hh04005
* creation date:   Thu May 04 17:20:07 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/


#ifndef EGML_STATISTIC__H_
#define EGML_STATISTIC__H_
//---------------------------------------------------------------	
//
// Statistic priority base-levels
//

#define eGML_STATLEVEL_WATCH        0           // can not be modified
#define eGML_STATLEVEL_POSCOUNT     10
#define eGML_STATLEVEL_NEGCOUNT     20

#define eGML_STATLEVEL_CLIPPING     30
#define eGML_STATLEVEL_DOTS         40
#define eGML_STATLEVEL_LINES        50
#define eGML_STATLEVEL_POLYGONS     60
#define eGML_STATLEVEL_PRIMITIVES   70
#define eGML_STATLEVEL_BLITS        80
#define eGML_STATLEVEL_TRANSFORM    90

#define eGML_STATLEVEL_INTERN       100

#define eGML_STATLEVEL_MAX          256         // only userdefined statistics above this

//---------------------------------------------------------------	
//
// Statistic and profiling groups
//

//---------------------------------------------------------------	
#ifndef eDBG_STATISTIC

/// All automatic watches
#define eGML_STATGROUP_WATCH            
#define eGML_STATGROUP_FPS              
#define eGML_STATGROUP_ALL              

#define eGML_STATGROUP_WATCH_ALL        

#define eGML_STATGROUP_COUNT_ALL        
#define eGML_STATGROUP_COUNT_POS        
#define eGML_STATGROUP_COUNT_NEG        
 
#define eGML_STATGROUP_DOTS             
#define eGML_STATGROUP_BLITS            
#define eGML_STATGROUP_LINES            
#define eGML_STATGROUP_POLYGONS         
#define eGML_STATGROUP_CLIPPING         
#define eGML_STATGROUP_TRANSFORM        
#define eGML_STATGROUP_PRIMITIVES       

#define eGML_STATGROUP_PROFILE


//---------------------------------------------------------------	
#else

#define eGML_STATGROUP_WATCH            eDBG_Group( eDBG_Level( eGML_STATLEVEL_WATCH ) )
#define eGML_STATGROUP_FPS              eDBG_Group( eDBG_Level( eGML_STATLEVEL_INTERN + 10 ) )
#define eGML_STATGROUP_ALL              eDBG_Group( eDBG_Range( eGML_STATLEVEL_WATCH, eGML_STATLEVEL_MAX ) )

#define eGML_STATGROUP_WATCH_ALL        eDBG_Group( eDBG_Range( eGML_STATLEVEL_WATCH, eGML_STATLEVEL_WATCH + 9 ) )

#define eGML_STATGROUP_COUNT_ALL        eDBG_Group( eDBG_Range( eGML_STATLEVEL_POSCOUNT, eGML_STATLEVEL_NEGCOUNT + 9 ) )
#define eGML_STATGROUP_COUNT_POS        eDBG_Group( eDBG_Range( eGML_STATLEVEL_POSCOUNT, eGML_STATLEVEL_POSCOUNT + 9 ) )
#define eGML_STATGROUP_COUNT_NEG        eDBG_Group( eDBG_Range( eGML_STATLEVEL_NEGCOUNT, eGML_STATLEVEL_NEGCOUNT + 9 ) )
 
#define eGML_STATGROUP_DOTS             eDBG_Group( eDBG_Range( eGML_STATLEVEL_DOTS, eGML_STATLEVEL_DOTS + 9 ) )
#define eGML_STATGROUP_BLITS            eDBG_Group( eDBG_Range( eGML_STATLEVEL_BLITS, eGML_STATLEVEL_BLITS + 9 ) )
#define eGML_STATGROUP_LINES            eDBG_Group( eDBG_Range( eGML_STATLEVEL_LINES, eGML_STATLEVEL_LINES + 9 ) )
#define eGML_STATGROUP_POLYGONS         eDBG_Group( eDBG_Range( eGML_STATLEVEL_POLYGONS, eGML_STATLEVEL_POLYGONS + 9 ) )
#define eGML_STATGROUP_CLIPPING         eDBG_Group( eDBG_Range( eGML_STATLEVEL_CLIPPING, eGML_STATLEVEL_CLIPPING + 9 ) )
#define eGML_STATGROUP_TRANSFORM        eDBG_Group( eDBG_Range( eGML_STATLEVEL_TRANSFORM, eGML_STATLEVEL_TRANSFORM + 9 ) )
#define eGML_STATGROUP_PRIMITIVES       eDBG_Group( eDBG_Range( eGML_STATLEVEL_PRIMITIVES, eGML_STATLEVEL_PRIMITIVES + 9 ) )

#define eGML_STATGROUP_PROFILE          eDBG_Group( 7, \
                                         eDBG_Range( eGML_STATLEVEL_INTERN, eGML_STATLEVEL_INTERN+9), \
                                         eDBG_Level( eGML_STATLEVEL_BLITS + 5 ), \
                                         eDBG_Level( eGML_STATLEVEL_CLIPPING + 5 ), \
                                         eDBG_Level( eGML_STATLEVEL_TRANSFORM + 5 ), \
                                         eDBG_Level( eGML_STATLEVEL_LINES + 5 ), \
                                         eDBG_Level( eGML_STATLEVEL_DOTS + 5 ), \
                                         eDBG_Level( eGML_STATLEVEL_POLYGONS + 5 ) )

#endif

//---------------------------------------------------------------	
#endif
