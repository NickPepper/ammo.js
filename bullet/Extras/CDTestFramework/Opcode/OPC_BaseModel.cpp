/*
 *	OPCODE - Optimized Collision Detection
 * http://www.codercorner.com/Opcode.htm
 * 
 * Copyright (c) 2001-2008 Pierre Terdiman,  pierre@codercorner.com

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains base model interface.
 *	\file		OPC_BaseModel.cpp
 *	\author		Pierre Terdiman
 *	\date		May, 18, 2003
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	The base class for collision models.
 *
 *	\class		BaseModel
 *	\author		Pierre Terdiman
 *	\version	1.3
 *	\date		May, 18, 2003
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Precompiled Header
#include "Stdafx.h"

using namespace Opcode;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
OPCODECREATE::OPCODECREATE()
{
	mIMesh				= null;
	mSettings.mRules	= SPLIT_SPLATTER_POINTS | SPLIT_GEOM_CENTER;
	mSettings.mLimit	= 1;	// Mandatory for complete trees
	mNoLeaf				= true;
	mQuantized			= true;
#ifdef __MESHMERIZER_H__
	mCollisionHull		= false;
#endif // __MESHMERIZER_H__
	mKeepOriginal		= false;
	mCanRemap			= false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Constructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BaseModel::BaseModel() : mIMesh(null), mModelCode(0), mSource(null), mTree(null)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Destructor.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BaseModel::~BaseModel()
{
	ReleaseBase();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Releases everything.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BaseModel::ReleaseBase()
{
	DELETESINGLE(mSource);
	DELETESINGLE(mTree);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Creates an optimized tree according to user-settings, and setups mModelCode.
 *	\param		no_leaf		[in] true for "no leaf" tree
 *	\param		quantized	[in] true for quantized tree
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool BaseModel::CreateTree(bool no_leaf, bool quantized)
{
	DELETESINGLE(mTree);

	// Setup model code
	if(no_leaf)		mModelCode |= OPC_NO_LEAF;
	else			mModelCode &= ~OPC_NO_LEAF;

	if(quantized)	mModelCode |= OPC_QUANTIZED;
	else			mModelCode &= ~OPC_QUANTIZED;

	// Create the correct class
	if(mModelCode & OPC_NO_LEAF)
	{
		if(mModelCode & OPC_QUANTIZED)	mTree = new AABBQuantizedNoLeafTree;
		else							mTree = new AABBNoLeafTree;
	}
	else
	{
		if(mModelCode & OPC_QUANTIZED)	mTree = new AABBQuantizedTree;
		else							mTree = new AABBCollisionTree;
	}
	CHECKALLOC(mTree);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Refits the collision model. This can be used to handle dynamic meshes. Usage is:
 *	1. modify your mesh vertices (keep the topology constant!)
 *	2. refit the tree (call this method)
 *	\return		true if success
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool BaseModel::Refit()
{
	// Refit the optimized tree
	return mTree->Refit(mIMesh);

// Old code kept for reference : refit the source tree then rebuild !
//	if(!mSource)	return false;
//	// Ouch...
//	mSource->Refit(&mTB);
//	// Ouch...
//	return mTree->Build(mSource);
}
