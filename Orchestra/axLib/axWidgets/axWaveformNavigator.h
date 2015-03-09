//
//  axWaveformNavigator.h
//  MidiSequencer
//
//  Created by Alexandre Arsenault on 2014-11-15.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#ifndef __MidiSequencer__axWaveformNavigator__
#define __MidiSequencer__axWaveformNavigator__

#include "axLib/axLib.h"


class axAudioBuffer;

/// @todo Format as a standard axWidget.
class axWaveformNavigator : public axPanel
{
public:
    axWaveformNavigator(axWindow* parent,
                        const axRect& rect,
                        const axSliderEvents& events);
    
    void SetAudioBuffer(axAudioBuffer* buffer);
    
//    void SetZoom(const double& zoom);
//
    void SetBorders(const double& left, const double& right);
    void SetBorders(const axFloatRange& borders);
    void SetLeftBorder(const double& pos);
    void SetRightBorder(const double& pos);
    void SetPlayingPos(const double& playing_pos);
    
    void SetValueChangeEvt(axEvtFunction(double) fct);
    
    axEVENT(double, OnPlayingPositionChange);
    
private:
    axAudioBuffer* _audioBuffer;
    
    double _leftBorder, _rightBorder;
    double _fillAlpha;
    double _playingPos;
    
    int _click_pos_delta_x;
    
    std::vector<axPoint> _waveformDrawingData;
    
    void FillWaveformDrawingData();
    axRect GetBorderRect() const;
    axFloatRange GetBorderRangeFromRect(const axRect& rect) const;
    
    axEvtFunction(double) _value_change_evt;
    
    // Events.
    virtual void OnPaint();
    virtual void OnMouseLeftDragging(const axPoint& pos);
    virtual void OnMouseLeftDown(const axPoint& pos);
    virtual void OnMouseLeftUp(const axPoint& pos);
    virtual void OnMouseEnter();
    virtual void OnMouseLeave();
    
    void OnPlayingPositionChange(const double& playing_pos);
};

#endif /* defined(__MidiSequencer__axWaveformNavigator__) */
