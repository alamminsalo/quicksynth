#include "waveform.h"

WaveForm::WaveForm()
{

}

bool WaveForm::setTable(StkFrames &table)
{
    m_table = table;
}

inline StkFrames& WaveForm::tick( StkFrames& frames, unsigned int channel )
{
#if defined(_STK_DEBUG_)
  if ( channel >= frames.channels() ) {
    oStream_ << "SineWave::tick(): channel and StkFrames arguments are incompatible!";
    handleError( StkError::FUNCTION_ARGUMENT );
  }
#endif

  StkFloat *samples = &frames[channel];
  StkFloat tmp = 0.0;

  unsigned int hop = frames.channels();
  for ( unsigned int i=0; i<frames.frames(); i++, samples += hop ) {

    // Check limits of time address ... if necessary, recalculate modulo
    // TABLE_SIZE.
    while ( time_ < 0.0 )
      time_ += TABLE_SIZE;
    while ( time_ >= TABLE_SIZE )
      time_ -= TABLE_SIZE;

    iIndex_ = (unsigned int) time_;
    alpha_ = time_ - iIndex_;
    tmp = m_table[ iIndex_ ];
    tmp += ( alpha_ * ( m_table[ iIndex_ + 1 ] - tmp ) );
    *samples = tmp;

    // Increment time, which can be negative.
    time_ += rate_;
  }

  lastFrame_[0] = tmp;
  return frames;
}
