/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
/*=========================================================================
 *
 *  Portions of this file are subject to the VTK Toolkit Version 3 copyright.
 *
 *  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
 *
 *  For complete copyright, license and disclaimer of warranty information
 *  please refer to the NOTICE file at the top of the ITK source tree.
 *
 *=========================================================================*/
#ifndef itkSliceSeriesSpecialCoordinatesImage_hxx
#define itkSliceSeriesSpecialCoordinatesImage_hxx
#include "itkSliceSeriesSpecialCoordinatesImage.h"

namespace itk
{

template< typename TSliceImage, typename TTransform, typename TPixel, unsigned int VDimension >
SliceSeriesSpecialCoordinatesImage< TSliceImage, TTransform, TPixel, VDimension >
::SliceSeriesSpecialCoordinatesImage()
{
  this->m_SliceImage = SliceImageType::New();
  this->m_SliceTransforms = SliceTransformsType::New();
}


template< typename TSliceImage, typename TTransform, typename TPixel, unsigned int VDimension >
void
SliceSeriesSpecialCoordinatesImage< TSliceImage, TTransform, TPixel, VDimension >
::SetLargestPossibleRegion( const RegionType & region )
{
  Superclass::SetLargestPossibleRegion( region );
  const SizeType & largestSize = this->GetLargestPossibleRegion().GetSize();
  this->m_SliceTransforms->Reserve( largestSize[ImageDimension - 1] );
}


template< typename TSliceImage, typename TTransform, typename TPixel, unsigned int VDimension >
void
SliceSeriesSpecialCoordinatesImage< TSliceImage, TTransform, TPixel, VDimension >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent
     << "SliceImage = " << m_SliceImage
     << std::endl;
  os << indent
     << "SliceTransforms = " << m_SliceTransforms
     << std::endl;
}


template< typename TSliceImage, typename TTransform, typename TPixel, unsigned int VDimension >
void
SliceSeriesSpecialCoordinatesImage< TSliceImage, TTransform, TPixel, VDimension >
::Graft(const DataObject *data)
{
  // call the superclass' implementation
  Superclass::Graft(data);

  if ( data )
    {
    // Attempt to cast data to a SliceSeriesSpecialCoordinatesImage
    const Self * const imgData = dynamic_cast< const Self * >( data );

    if ( imgData )
      {
      // Now copy anything remaining that is needed
      this->SetPixelContainer( const_cast< PixelContainer * >
                               ( imgData->GetPixelContainer() ) );
      }
    else
      {
      // pointer could not be cast back down
      itkExceptionMacro( << "itk::Image::Graft() cannot cast "
                         << typeid( data ).name() << " to "
                         << typeid( const Self * ).name() );
      }
    }
}

} // end namespace itk

#endif
