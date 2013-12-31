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
#ifndef __sitkImageFilter_h
#define __sitkImageFilter_h

#include "sitkMacro.h"
#include "sitkMemberFunctionFactory.h"
#include "sitkBasicFilters.h"
#include "sitkImageFilterExecuteBase.h"

namespace itk {

  namespace simple {

  /** \class ImageFilter
   * \brief The base interface for SimpleITK filters that take one input image
   *
   * All SimpleITK filters which take one input image should inherit from this
   * class
   */
  template < unsigned int N>
  class SITKBasicFilters0_EXPORT ImageFilter:
      public ImageFilterExecuteBase<N>
  {
    public:
      typedef ImageFilter Self;

      //
      // Type List Setup
      //

      //
      // Filter Setup
      //

      /**
       * Default Constructor that takes no arguments and initializes
       * default parameters
       */
      ImageFilter();

      /**
       * Default Destructor
       */
      virtual ~ImageFilter() = 0;

    protected:

      template< class TImageType >
      static typename TImageType::ConstPointer CastImageToITK( const Image &img )
      {
        typename TImageType::ConstPointer itkImage =
          dynamic_cast < const TImageType* > ( img.GetITKBase() );

        if ( itkImage.IsNull() )
          {
          sitkExceptionMacro( "Unexpected template dispatch error!" );
          }
        return itkImage;
      }

      // Simple ITK must use a zero based index
      template< class TImageType>
      static void FixNonZeroIndex( TImageType * img )
      {
        assert( img != NULL );

        typename TImageType::RegionType r = img->GetLargestPossibleRegion();
        typename TImageType::IndexType idx = r.GetIndex();

        for( unsigned int i = 0; i < TImageType::ImageDimension; ++i )
          {

          if ( idx[i] != 0 )
            {
            // if any of the indcies are non-zero, then just fix it
            typename TImageType::PointType o;
            img->TransformIndexToPhysicalPoint( idx, o );
            img->SetOrigin( o );

            idx.Fill( 0 );
            r.SetIndex( idx );

            // Need to set the buffered region to match largest
            img->SetRegions( r );

            return;
            }
          }

      }

    };


  }
}
#endif
