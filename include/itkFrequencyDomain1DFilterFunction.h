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
#ifndef itkFrequencyDomain1DFilterFunction_h
#define itkFrequencyDomain1DFilterFunction_h

#include "itkObject.h"

namespace itk
{
/** \class FrequencyDomain1DImageFilter
 * \brief
 * Class to implment filter functions for FrequencyDomain1DImageFilter
 * 
 * \ingroup FourierTransform
 * \ingroup Ultrasound
 */
class FrequencyDomain1DFilterFunction:
  public Object 
{
public:

  /** Standard class typedefs. */
  typedef FrequencyDomain1DFilterFunction      Self;
  typedef Object                               Superclass;
  typedef SmartPointer< Self >                 Pointer;
  typedef SmartPointer< const Self >           ConstPointer;

  itkTypeMacro( FrequencyDomain1DFilterFunction, Object);
  itkNewMacro( Self );


  /** discrete frequency index such as retunr by FFTW, i.e.
   * 0 is DC component
   * i / SignalSize * 2*pi is the frequency, 
   * i.e., first half positive frequencies and 
   * second half negative frequencies in reverse order.
   */
  double EvaluateIndex(SizeValueType &i)
    {
    if( m_UseCache )
      {
      //TODO: Check for out of bounds?
      return m_Cache[i];
      }
    else
      {
      return this->EvaluateFrequency( this->GetFrequency(i) );
      }
    }

  void SetSignalSize( const SizeValueType &size)
    {
    if( this->m_SignalSize != size )
      {
      this->m_SignalSize = size;
      if( this->m_UseCache )
        {
        this->m_Cache.resize( size );
        }
      this->Modified();
      }
    }

  SizeValueType GetSignalSize()
    {
    return m_SignalSize;
    }

   itkSetMacro( UseCache, bool );
   itkGetMacro( UseCache, bool );

  /** 
   * Override this function to implement a specific filter.
   * The input ranges from -1 to 1 
   * Default is identity function. 
   */
  virtual double EvaluateFrequency(double frequency)
    {
    return 1.0;
    };

  virtual void Modified( ) const ITK_OVERRIDE
    {
    //TODO: is there a better way to update the cache
    const_cast< FrequencyDomain1DFilterFunction *>(this)->UpdateCache();
    Superclass::Modified();
    }

protected:

  virtual void PrintSelf( std::ostream& os, Indent indent ) const ITK_OVERRIDE
    {
    Superclass::PrintSelf( os, indent );

    os << indent << "SignalSize: " << m_SignalSize << std::endl;
    os << indent << "UseCache: " << m_UseCache << std::endl;
    os << indent << "CacheSize: " << m_Cache.size() << std::endl;
    }

private:
  
  double GetFrequency(SizeValueType &i)
    {
    double f = (2.0 * i ) / m_SignalSize; 
    if( f > 1.0 )
      {
      f = f - 2.0; 
      }
    return f;
    }
 
  void UpdateCache()
    {
    if( this->m_UseCache )
      {
      for( SizeValueType i=0; i < m_Cache.size(); i++)
        {
        this->m_Cache[i] = this->EvaluateFrequency( this->GetFrequency(i) );          
        }
      }
    }
 
  bool m_UseCache = true;
  std::vector< double > m_Cache;
  SizeValueType m_SignalSize = 0;
};

}
#endif // itkFrequencyDomain1DFilterFunction.h
