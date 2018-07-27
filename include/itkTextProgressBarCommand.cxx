#include "itkTextProgressBarCommand.h"

#include "itkProcessObject.h"

#include <iostream>

namespace itk
{

TextProgressBarCommand
::TextProgressBarCommand():
  m_Progress(   "[>                                                  ]" )
{
}

void
TextProgressBarCommand
::Execute(itk::Object *caller, const itk::EventObject & event)
{
  this->Execute( (const itk::Object *)caller, event);
}


void
TextProgressBarCommand
::Execute(const itk::Object * object, const itk::EventObject & event)
{
  if( !(itk::ProgressEvent().CheckEvent( &event ) ) )
    return;

  itk::ProcessObject::ConstPointer process = dynamic_cast< const itk::ProcessObject * > ( object );
  if( !process )
    return;
  double progress = process->GetProgress();

  const unsigned int position = static_cast< unsigned int >( progress * 50 );

  unsigned int i;
  for( i = 0; i < position; i++ )
    m_Progress[i+1] = '=';

  if( position == 50 )
    m_Progress[position+1] = '=';
  else
    m_Progress[position+1] = '>';

  for( i = position + 1; i < 51; i++ )
    m_Progress[i+1] = ' ';

  std::cout << process->GetNameOfClass() << ": ";
  std::cout << m_Progress << std::endl;
  // If you are in a console, this is nicer because you don't have repeats.  But
  // in ctest -V nothing shows up.
  //std::cout << m_Identifier << ": ";
  //std::cout << m_Progress << '\r' << std::flush;

  if( position == 50 )
    std::cout << std::endl;
}

} // end namespace itk
