/****************************************************************************
* copyright:       TES Electronic Solutions GmbH
* project:         eGML
* file name:       eGML_PolydrawAA.h
* %version:        134.0 %
*
* author:          hh04005
* creation date:   Thu May 04 17:18:28 2006
* description:

* %date_modified:  % (%derived_by: hh04005 %)
*
* history:
* date         | name          | description of modifications
*-------------------------------------------------------------------------
  05-08-03     | hh04031       | Exchange fileheader with CM Synergy-Tags

******************************************************************************/

#ifndef EGML_POLYDRAWAA__H_
#define EGML_POLYDRAWAA__H_
//---------------------------------------------------------------	

#ifdef  eGML_GAMMACORRECTAA
#define BlendPixelGamma( p, c, a )      BlendPixel( p, c, g_eGML_ubGammaRamp[ (eC_UByte) (a) ] )
#else
#define BlendPixelGamma( p, c, a )      BlendPixelSrcAlpha( p, c, a )
#endif


#define eGML_POLYAA_DISTBITS 5
#define eGML_POLYAA_NORMBITS 4

struct eGML_AAEdgeBucket
{
  eC_Int    ybot;
  eC_Value  X1;
  eC_Int    normalmask;      // coverage mask, angle part
  eC_Int    dist;		        // current distance from edge, fixed point: 1.5.26 rescaled by 1/(sqrt(2)/2)
  eC_Int    nx,ny;		        // normal x and y components,  fixed point: 1.5.26
  eC_Value  xslope;          // x-slope to advance an integer y-position
  eC_Value  xipol;           // x-position in current scanline (intersection w. bottom edge of scanline)
  eC_Int    xsleft,xsright;  // xstart, xstop in current scanling. interval end (xsright) is INCLUSIVE
  eGML_AAEdgeBucket *next;
  eGML_AAEdgeBucket *AEnext;
  eC_Int    cliptop;         // Clipmask for top clip
  eC_Int    clipbot;         // Clipmask for bottom clip
  eC_Int    edgeflags;       // bit2: "dirty" flag bit1: current line is end of edge bit0: current line is begin of edge
};

typedef eGML_AAEdgeBucket *eGML_AAEdgeBucketP;

//---------------------------------------------------------------	
extern eC_UInt  g_auiEdgemasks[2048];
extern eC_UByte g_aucEdgeArea[2048];
extern eC_UInt  g_auiClipmasks[16];
extern eC_UByte g_aucPoplookup[33];



//---------------------------------------------------------------	

extern void InitAAPolydraw();

extern bool SetupAAPolygon( eC_Int iMaxY, eC_Int iMaxX, eGML_AAEdgeBucket *Buckets, const eGML_Coord2D *pkCoords, eC_UInt uiPitch, eGML_AAEdgeBucket **ScanList, int &ystart, eC_UInt uiNb );


extern eGML_AAEdgeBucket * eGML_PolyGetAABucketBuffer( eC_UInt uiQuantity );

extern eGML_AAEdgeBucketP * eGML_PolyGetScanBuffer( eC_UInt uiQuantity );

//---------------------------------------------------------------	
inline eC_UInt int_ones(eC_UInt x)
{
    x -= ((x >> 1) & 0x55555555);
    x = (((x >> 2) & 0x33333333) + (x & 0x33333333));
    x = (((x >> 4) + x) & 0x0f0f0f0f);
    x += (x >> 8);
    x += (x >> 16);
    return (x & 0x0000003f);
}

//---------------------------------------------------------------
template <
typename P1,		// Pixeltype
typename O1,		// Offsettype
class A1,			// Accessmethods for target
eC_Bool bMustBlendAA
>
class eGML_Polydraw_AA
{
public:

	static void Polygon(eGML_Bitmap *pkBitmap, const void *pvValues, eC_UInt uiNb, eC_UInt uiPitch, P1 pxColor, eC_UByte ubAlpha )
	{	
    eC_UInt uiLines = pkBitmap->GetHeight() + 2;
    eGML_AAEdgeBucket *Buckets   = eGML_PolyGetAABucketBuffer( uiNb );
		eGML_AAEdgeBucket **ScanList = eGML_PolyGetScanBuffer( uiLines );
		const eGML_Coord2D *pkCoords = (const eGML_Coord2D *)pvValues;


		// Edge preprocessing - compute slopes and build scan line buckets

		// Some parameters have only to be computed once per edge, this could take place during the
		// scanline bucket building or while inserting into the active edge table. The latter
		// reduces the amount of memory required for the scanline buckets, while the former
		// may help balance the code length of both loops to improve cache usage.



		int ystart;


    if (!SetupAAPolygon( pkBitmap->GetClipMaxY(), pkBitmap->GetClipMaxX(), Buckets, pkCoords, uiPitch, ScanList, ystart, uiNb ))
    {
      eC_Memset(ScanList, 0, sizeof(eGML_AAEdgeBucket *) * uiLines);
      return;
    }


		// Scan conversion main loop

		// For each scanline do:
		//
		// 1)  Add new edges for this scanline to active edge table (AET), insert sort according to x-start of edge span
		// 2)  Rasterize two possible types of spans: "edge spans" that are part of an edge and normal spans, that lie within
		//     the interior of the polygon. Multiple edge spans may be superimposed, normal spans are detected using a parity test.
		// 3a) Remove edges ending in the current scanline. Note: Spans can be removed in the same scanline they were added when
		//     their y-extend is smaller than one. In contrast to non-antialiasing rasterizers they may not be neglected.
		// 3b) Advance edge x-positions to next line.
		// 3c) Check for self overlap, re-sort if necessary. At this point there should be at least two spans in the AET

		int currentY=ystart;
		eGML_AAEdgeBucket *AETstart=0;
		O1 kOff;
		do {

			// 1) Add new edges to AET, if present

			eGML_AAEdgeBucket *curbucket=ScanList[currentY];
			ScanList[currentY]=0;                         // delete entry, so table is clean for next polygon

			while (curbucket)
			{
				// Ok, found a new edge for this scanline, now lets insert it
				eGML_AAEdgeBucket **AETinsert=&AETstart;

				// Insert new edge so that xsleft is sorted in ascending order
				// ATT: Edges are inserted according to the intersection interval of the first scanline.
				//      Therefore no disambiguation is possible for two edges starting at the same vertex.
				//      This could be fixed by comparing the slopes, however it is already taken care of
				//      in the AET re-sort.

				while (*AETinsert)
				{
					if (AETinsert[0]->xsleft>curbucket->xsleft) break; // simple way without disambiguation
					AETinsert=&(AETinsert[0]->AEnext);
				}
				curbucket->AEnext=AETinsert[0];
				AETinsert[0]=curbucket;

				curbucket=curbucket->next;
			}

			// 2) Rasterize
			//
			//  a) If current xpos<current span.xsleft -> draw or skip
			//  b) for xpos: Iterate through spans from current span until xpos<span.xsleft or spans out
			//     for each span:  -if xpos<=span.xsright, do AA
			//                  :  -if xpos==span.xsright && newedge=false, toggle inpoly
			//  c) if current span.xsright==xpos, advance span, loop

			int xpos=AETstart->xsleft;
			eGML_AAEdgeBucket *scanspan=AETstart;
			int  filledlines=0;  // Subpixel XOR fill
			int  filledlinesdelay=0;


			A1::GetOffset(pkBitmap, xpos, currentY, kOff);

			while (scanspan)
			{
				// a) Draw/Skip connected spans that are not on an edge

				if (xpos<scanspan->xsleft)
				{
					int len=scanspan->xsleft-xpos;

					if (filledlines)
					{
						if(bMustBlendAA || (ubAlpha != 0xff))
						{
							// transparent span
							while (len-- >0)
							{
								A1::BlendPixelGamma( kOff, pxColor, ubAlpha );
								A1::IncX( pkBitmap, kOff );
							}
						} 
                  else
						{
                     A1::PutSpan( kOff, pxColor, len);
						}
					}
          else
          {
            A1::AddOffset( pkBitmap, kOff, len, 0, kOff );
//            A1::GetOffset(pkBitmap, scanspan->xsleft, currentY, kOff);
					}

					xpos=scanspan->xsleft;

				}

				// b) draw edge fragments

				// "Hacked" version
				//
				// Has to deal with inner edges and special case where just one edge is on the current pixel
				//
				// IDEA:
				//
				//  b1) Deal with first fragment span
				//  b2) Search for next fragment, found one?
				//      NO,  read lookup for one mask, skip rest
				//      YES, table lookup for brightness continue to b6
				//  b3) Read first Mask, set inconvex flag for first fragment span
				//  b4) Loop and combine remaining fragments
				//  b5) Population count on bitmask
				//  b6) Iterate first span?

				int  mask=0;
				bool singlefragment=true;
				eGML_AAEdgeBucket *spaniter=scanspan->AEnext; // Next span

				// b2) Search for next fragment on this pixel

				while(spaniter)
				{
					if (xpos<spaniter->xsleft) break;
					if (xpos<=spaniter->xsright)   // Found another edge fragment on the current pixel
					{
						singlefragment=false;        // spaniter does now point to next valid edge fragment
						break;
					}
					spaniter=spaniter->AEnext;
				}

				// Find and combine subpixel masks for all edge intersection spans

				// edge intersection span ends here, update fillmask for first intersection span
				if (xpos==scanspan->xsright)
				{
					switch (scanspan->edgeflags&0x03)
					{
					case 0x00: filledlines^=0x3fffffff;                          break;  // mid edge
					case 0x01: filledlines^=scanspan->cliptop;                   break;  // edge starts in this scanline
					case 0x02: filledlines^=scanspan->clipbot;                   break;  // edge ends in this scanline
					case 0x03: filledlines^=scanspan->cliptop&scanspan->clipbot; break;  // edge starts and ends in this scanline
					}
				}

				eC_UByte coverage=0;

				eC_Int dtrunc;
				dtrunc=((eC_Int)scanspan->dist>>(26-eGML_POLYAA_DISTBITS+1))&0x1f;
				scanspan->dist+=scanspan->nx;   // advance first edge fragment distance

				if (singlefragment)
				{              // Ok, just a single edge intersecting this pixel, special case optimizations apply
					if ((scanspan->edgeflags&0x04)!=0x04)
					{
						dtrunc=filledlinesdelay?(-dtrunc)&0x1f:dtrunc;
						coverage=g_aucEdgeArea[scanspan->normalmask+dtrunc];
						#ifndef eGML_POLYGONALPHA_DISABLE
						if (ubAlpha != 0xff) coverage = (coverage * ubAlpha) >> 8; // could use min here as approx
						#endif
					} else {
						mask=g_auiEdgemasks[scanspan->normalmask+dtrunc];
						if (scanspan->edgeflags&0x01) mask&=scanspan->cliptop;   // spans may have to be clipped, even for single edges!
						if (scanspan->edgeflags&0x02) mask&=scanspan->clipbot;

						mask^=filledlinesdelay;
						coverage=g_aucPoplookup[int_ones(mask)];
						#ifndef eGML_POLYGONALPHA_DISABLE
						if (ubAlpha != 0xff) coverage = (coverage * ubAlpha) >> 8; // could use min here as approx
						#endif
					}
				} else {

					// Mask for first edge span
					mask=g_auiEdgemasks[scanspan->normalmask+dtrunc];

					if (scanspan->edgeflags&0x01) mask&=scanspan->cliptop;   // spans may have to be clipped, even for single edges!
					if (scanspan->edgeflags&0x02) mask&=scanspan->clipbot;

					scanspan->edgeflags|=0x04;  // set dirty
					do
					{
						if (xpos<=spaniter->xsright)  // Always true for first check, redundant in first loop iteration!
						{
							eC_Int tempmask;
							dtrunc=((eC_Int)spaniter->dist>>(26-eGML_POLYAA_DISTBITS+1))&0x1f;
							tempmask=g_auiEdgemasks[spaniter->normalmask+dtrunc];

							if (spaniter->edgeflags&0x01) tempmask&=spaniter->cliptop;   // spans may have to be clipped, even for single edges!
							if (spaniter->edgeflags&0x02) tempmask&=spaniter->clipbot;

							mask^=tempmask;

							if (xpos==spaniter->xsright)
							{
								switch (spaniter->edgeflags&0x03)
								{
								case 0x00: filledlines^=0x3fffffff;                          break;  // mid edge
								case 0x01: filledlines^=spaniter->cliptop;                   break;  // edge starts in this scanline
								case 0x02: filledlines^=spaniter->clipbot;                   break;  // edge ends in this scanline
								case 0x03: filledlines^=spaniter->cliptop&spaniter->clipbot; break;  // edge starts and ends in this scanline
								}
							}

							spaniter->edgeflags|=0x04;  // set dirty
							spaniter->dist+=spaniter->nx;   // advance edge fragment distance
						}
						spaniter=spaniter->AEnext;
						if (!spaniter) break;
					} while (xpos>=spaniter->xsleft);

					// XOR fill across the pixels, filling within pixels is taken care of by the masks
					mask^=filledlinesdelay;

					coverage=g_aucPoplookup[int_ones(mask)];
					#ifndef eGML_POLYGONALPHA_DISABLE
					if (ubAlpha != 0xff) coverage = (coverage * ubAlpha) >> 8; // could use min here as approx
					#endif
				}

				filledlinesdelay=filledlines;

				// c)
				// Traverse edges in AET

				while (scanspan->xsright<=xpos)
				{
					scanspan=scanspan->AEnext;
					if (!scanspan) break;
				}
	
				A1::BlendPixelSrcAlpha( kOff, pxColor, coverage );   

				xpos++;
				A1::IncX( pkBitmap, kOff );
			}

			// 3ab) Remove and advance active edges

			eGML_AAEdgeBucket **AETscan=&AETstart;

			while (*AETscan)
			{
				eGML_AAEdgeBucket *curedge=AETscan[0];
				int xsend;

				// Edge expired? -> Remove edge
				if (curedge->ybot==currentY)
				{
					*AETscan=AETscan[0]->AEnext;
					continue;
				}

				// Advance x-position of scanline
				// Update intersection interval for the _next_ scanline

				if (curedge->ybot==(currentY+1)) // next scaline is last line of edge
				{
					curedge->edgeflags=0x02;

#ifdef eC_FIXPOINT
					xsend=(eC_Int)curedge->X1>>16;
#else
					// Fix rounding error/ambigiousity for floats
					// If the same problem appears for fixed point, the same fix has to be apllied there

					xsend=(eC_Int)(eC_ToInt(eC_Floor(curedge->X1)));

					if (curedge->xslope>0)
					{
						if (curedge->xsright>xsend) // End of interpolation and real endpoint do not match due to numerical inaccuracy
						{                           // FIX: Interpolate over both real and erronous endpoint
							curedge->xsright--;

							curedge->dist-=curedge->nx;
						}
					} else {
						if (curedge->xsleft<xsend)
						{
							curedge->xsleft++;
						}
					}

#endif

				} else {
					curedge->edgeflags=0x00;
					curedge->xipol+=curedge->xslope;
					//        xsend=(int)(floor(curedge->xipol+0.0000001f)); // ATT: If broken spans appear, this has to be reimplemented in eC_Value
					xsend=(int)(eC_ToInt(eC_Floor(curedge->xipol)));
          if (xsend < 0)
          {
#ifdef EGML_POLYDRAW_DEBUG
             eDBG_DP("xsend (3)< 0");
#endif //EGML_POLYDRAW_DEBUG
            xsend  = 0;
          }
				}

				if (curedge->xslope>0) // update intersection interval
				{
					curedge->xsleft=curedge->xsright;
					curedge->xsright=xsend;
					curedge->dist+=curedge->ny-curedge->nx; // edge plane interpolation
				} else {
					int stepback=curedge->xsright-xsend;

					curedge->xsright=curedge->xsleft;
					curedge->xsleft=xsend;

					curedge->dist+=curedge->ny-curedge->nx*(stepback+1);
					// edge plane interpolation, has to step back current and next intersection interval!

				}
				AETscan=&(AETscan[0]->AEnext);
			}

			if (!AETstart)
        break;  // early out, so the sorting pass has at least one span to look at

			// 3c) Reordner edges in AET, in case self intersection or ambiguous insert took place
			//      Algorithm is an awkward implementation, rewrite!

			eGML_AAEdgeBucket **sortspan=&AETstart;

			while (sortspan[0]->AEnext) // requires at least one span in AET, stop when less than two are left
			{
				if (sortspan[0]->xsleft>sortspan[0]->AEnext->xsleft)
				{
					eGML_AAEdgeBucket *temp;
					eGML_AAEdgeBucket **trav;

					temp=sortspan[0]->AEnext;  // current edge
					sortspan[0]->AEnext = sortspan[0]->AEnext->AEnext; // remove current edge

					// Find new position to insert edge
					trav=&AETstart;
					while (trav[0])
					{
						if (trav[0]->xsleft>temp->xsleft) break;
						trav=&(trav[0]->AEnext);
					}
					// insert edge
					temp->AEnext=trav[0];
					trav[0]=temp;

				}
				else
				{
					sortspan = &(sortspan[0]->AEnext);
				}
			}

			currentY++;
		} while ( currentY <= (int)pkBitmap->GetClipMaxY()  );


    ScanList[currentY]               =0; //Save
    ScanList[pkBitmap->GetClipMaxY()]=0; //Save

  }
};

//---------------------------------------------------------------	
#endif
