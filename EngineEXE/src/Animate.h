#pragma once
#include "BasicIncludes.h"
#include "QuackDataTypes.h"

class Actor;

class Animate 
{
private: 
	/// <summary>
	/// Setting up a dictionary to map a frame at a location on the spritesheet to how long it should be played.
	/// Default play time is 1 second
	/// </summary>
	void GenerateFrameList();

public:
	Animate(Actor* target, const AnimationData& animationData);
	~Animate();

	/// <summary>
	/// Animates a sprite using a row from a sprite sheet
	/// </summary>
	/// <param name="deltaTime">Delta time</param>
	/// <param name="playRate">Adjusts time taken to play frame by dividing frame duration by playrate</param>
	void UpdateTextCoord(float deltaTime);
	/// <summary>
	/// Edits how long a frame plays for within the spritesheet
	/// </summary>
	/// <param name="row">Row of frame to edit</param>
	/// <param name="column">Column of frame to edit</param>
	/// <param name="duration">New duration to play frame at location</param>
	void SetFramePlayTime(int row, int column, float duration);
	void SetRowToPlay(int row);
	/// <summary>
	/// Changes the played duration of a frame at a given row and column
	/// </summary>
	/// <param name="row">Row of frame to edit</param>
	/// <param name="column">Column of frame to edit</param>
	/// <param name="time">New duration to play frame</param>
	void EditFramePlayTime(int row, int column, float time);


	/// <summary>
	/// Gets total playtime of animation from a given row in the sprite sheet
	/// </summary>
	/// <param name="row"></param>
	/// <returns>Playtime in seconds</returns>
	float GetAnimationPlayTime(int row);

	inline int GetRowToPlay() { return m_rowToPlay; }

	virtual inline void SetAnimationStatus(const bool newAnimating) { m_animationData.banimated = newAnimating; }

	virtual void SetAnimationRowData(std::vector<AnimationRowData> newAnimationRowData);
	virtual void SetAnimation(const AnimationRowData& newAnimation);
	virtual void SetAnimationName(const std::string newName);
	virtual void SetAnimationRowNumber(const int newRowNumber);
	virtual void SetAnimationNumberOfColumns(const int newNumberOfColumns);
	virtual void SetAnimationPlayRate(const float newPlayRate);
	virtual void SetAnimationLooping(const bool newbLooping);
	virtual void SetAnimationTotalRows(const int newTotalRows);
	virtual void SetAnimationTotalColumns(const int newTotalColumns);

	virtual inline const AnimationRowData& GetCurrentAnimation() { return m_currentAnimationData; }

	inline void ResetSpriteFrame() { m_spriteFrame = 0; }

private:
	Actor* m_object;

	std::map<std::pair<int, int>, float> m_durationData;

	std::pair<int, int> m_frameToPlay;

	AnimationData m_animationData = AnimationData();

	AnimationRowData m_currentAnimationData = AnimationRowData();

	float m_delay;
	int m_spriteFrame;
	int m_rowToPlay ;
	int m_amountOfColumns;

	bool m_bactive = false;
};
