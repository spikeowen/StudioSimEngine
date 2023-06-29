#include "pch.h"

#include "Animate.h"
#include "GameObject.h"
#include "Actor.h"

Animate::Animate(Actor* target, const AnimationData& animationData) : m_animationData(animationData)
{
	m_object = target;

	m_delay = 50.0f;
	m_spriteFrame = 0;

	m_rowToPlay = 0;

	GenerateFrameList();
}

Animate::~Animate()
{
	
}


void Animate::UpdateTextCoord(float deltaTime)
{
	if (!m_bactive)
	{
		return;
	}

	//Use delay to adjust play rate of the animation
	m_delay = m_delay + deltaTime;

	if (m_delay > 1.0f/m_currentAnimationData.playRate)
	{
		//Calculate starting position in sprite sheet based of the current frame
		glm::vec2 startLocation = glm::vec2(m_frameToPlay.second, m_frameToPlay.first);

		//making sure the sprite frame value isn't more than the number of columns
		startLocation.x /= m_animationData.totalColumns;
		startLocation.y /= m_animationData.totalRows;

		glm::vec2 endLocation = glm::vec2(m_frameToPlay.second + 1, m_frameToPlay.first + 1);
		endLocation.x /= m_animationData.totalColumns;
		endLocation.y /= m_animationData.totalRows;

		//Updating texture co-ordinates
		m_object->GetGameObjectData()->texCoords =
		{
			startLocation.x,	startLocation.y,
			endLocation.x,		startLocation.y,
			endLocation.x,	    endLocation.y,
			endLocation.x,	    endLocation.y,
			startLocation.x,	endLocation.y,
			startLocation.x,	startLocation.y
		};

		m_object->UpdateVertexArray();

		m_spriteFrame++;
		if (m_spriteFrame == m_currentAnimationData.amountOfColumns - 1)
		{
			if (!m_currentAnimationData.blooping)
			{
				if (m_currentAnimationData.name == "die")
				{
					m_bactive = false;
					return;
				}
				else
				{
					m_object->OnAnimationFinished(m_currentAnimationData);
				}
			}
		}
		m_spriteFrame = m_spriteFrame > m_currentAnimationData.amountOfColumns - 1 ? 0 : m_spriteFrame;
		m_frameToPlay = { m_rowToPlay, m_spriteFrame };
		m_delay = 0;
	}
}


void Animate::GenerateFrameList()
{
	float time = 1.0f;

	for (int row = 0; row < m_animationData.totalRows; ++row)
	{
		for (int column = 0; column < m_animationData.totalColumns; ++column)
		{
			std::pair<int, int> location = { row, column };

			m_durationData[location] = time;
		}
	}
}


void Animate::SetFramePlayTime(int row, int column, float duration)
{
	std::pair<int, int> location = { row, column };

	m_durationData[location] = duration;
}

void Animate::SetRowToPlay(int row)
{
	m_rowToPlay = row;
	m_spriteFrame = 0;
	m_delay = 50.0f;
}


void Animate::EditFramePlayTime(int row, int column, float time)
{
	std::pair<int, int> frameLocation = { row, column };

	m_durationData[frameLocation] = time;
}


float Animate::GetAnimationPlayTime(int row)
{
	/*float playtime = 0.0f;

	for (int column = 0; column < m_animationData.columns; ++column)
	{
		std::pair<int, int> frame = { row, column };
		playtime += m_durationData[frame];
	}

	return playtime;*/
	return 0;
}

void Animate::SetAnimationRowData(std::vector<AnimationRowData> newAnimationRowData)
{
	m_animationData.animationRowData = newAnimationRowData;
}

void Animate::SetAnimation(const AnimationRowData& newAnimation)
{
	m_spriteFrame = 0;
	m_currentAnimationData = newAnimation;
	m_rowToPlay = newAnimation.rowNumber;
	m_amountOfColumns = newAnimation.amountOfColumns;

	for (int i = 0; i < newAnimation.amountOfColumns; i++)
	{
		EditFramePlayTime(newAnimation.rowNumber, i, newAnimation.playRate);
	}

	m_bactive = true;
}

void Animate::SetAnimationName(const std::string newName)
{
	m_currentAnimationData.name = newName;
	SetAnimation(m_currentAnimationData);
}

void Animate::SetAnimationRowNumber(const int newRowNumber)
{
	m_currentAnimationData.rowNumber = newRowNumber;
	SetAnimation(m_currentAnimationData);
}

void Animate::SetAnimationNumberOfColumns(const int newNumberOfColumns)
{
	m_currentAnimationData.amountOfColumns = newNumberOfColumns;
	SetAnimation(m_currentAnimationData);
}

void Animate::SetAnimationPlayRate(const float newPlayRate)
{
	m_currentAnimationData.playRate = newPlayRate;
	SetAnimation(m_currentAnimationData);
}

void Animate::SetAnimationLooping(const bool newbLooping)
{
	m_currentAnimationData.blooping = newbLooping;
	SetAnimation(m_currentAnimationData);
}

void Animate::SetAnimationTotalRows(const int newTotalRows)
{
	m_animationData.totalRows = newTotalRows;
}

void Animate::SetAnimationTotalColumns(const int newTotalColumns)
{
	m_animationData.totalColumns = newTotalColumns;
}
