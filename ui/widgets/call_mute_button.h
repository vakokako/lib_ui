// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#pragma once

#include "base/unique_qptr.h"
#include "ui/effects/animations.h"
#include "ui/effects/cross_line.h"
#include "ui/effects/gradient.h"
#include "ui/effects/radial_animation.h"

namespace Ui {

class BlobsWidget;

class AbstractButton;
class FlatLabel;
class RpWidget;
class AnimatedLabel;

struct CallButtonColors;

enum class CallMuteButtonType {
	Connecting,
	Active,
	Muted,
	ForceMuted,
};

struct CallMuteButtonState {
	QString text;
	QString subtext;
	CallMuteButtonType type = CallMuteButtonType::Connecting;
};

class CallMuteButton final {
public:
	explicit CallMuteButton(
		not_null<RpWidget*> parent,
		rpl::producer<bool> &&hideBlobs,
		CallMuteButtonState initial = CallMuteButtonState());
	~CallMuteButton();

	void setState(const CallMuteButtonState &state);
	void setLevel(float level);
	[[nodiscard]] rpl::producer<Qt::MouseButton> clicks() const;

	[[nodiscard]] QSize innerSize() const;
	[[nodiscard]] QRect innerGeometry() const;
	void moveInner(QPoint position);

	void shake();

	void setVisible(bool visible);
	void show() {
		setVisible(true);
	}
	void hide() {
		setVisible(false);
	}
	void raise();
	void lower();

	[[nodiscard]] rpl::producer<CallButtonColors> colorOverrides() const;

	[[nodiscard]] rpl::lifetime &lifetime();

private:
	enum class HandleMouseState {
		Enabled,
		Blocked,
		Disabled,
	};
	struct RadialInfo {
		std::optional<RadialState> state = std::nullopt;
		bool isDirectionToShow = false;
		rpl::variable<float64> rawShowProgress = 0.;
		float64 realShowProgress = 0.;
		const style::InfiniteRadialAnimation &st = st::callConnectingRadial;
	};
	void init();
	void overridesColors(
		CallMuteButtonType fromType,
		CallMuteButtonType toType,
		float64 progress);

	void setHandleMouseState(HandleMouseState state);
	void updateCenterLabelGeometry(QRect my, QSize size);
	void updateLabelGeometry(QRect my, QSize size);
	void updateSublabelGeometry(QRect my, QSize size);
	void updateLabelsGeometry();

	[[nodiscard]] static HandleMouseState HandleMouseStateFromType(
		CallMuteButtonType type);

	rpl::variable<CallMuteButtonState> _state;
	float _level = 0.;
	float64 _crossLineProgress = 0.;
	QRect _muteIconRect;
	HandleMouseState _handleMouseState = HandleMouseState::Enabled;

	const style::CallButton &_st;

	const base::unique_qptr<BlobsWidget> _blobs;
	const base::unique_qptr<AbstractButton> _content;
	const base::unique_qptr<AnimatedLabel> _centerLabel;
	const base::unique_qptr<AnimatedLabel> _label;
	const base::unique_qptr<AnimatedLabel> _sublabel;
	int _labelShakeShift = 0;

	RadialInfo _radialInfo;
	std::unique_ptr<InfiniteRadialAnimation> _radial;
	const base::flat_map<CallMuteButtonType, anim::gradient_colors> _colors;

	CrossLineAnimation _crossLineMuteAnimation;
	Animations::Simple _switchAnimation;
	Animations::Simple _shakeAnimation;

	rpl::event_stream<CallButtonColors> _colorOverrides;

};

} // namespace Ui
