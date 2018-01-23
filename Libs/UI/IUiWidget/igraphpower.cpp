﻿#include "igraphpower.h"
#include "igraphpower_p.h"

IGraphPowerPrivate::IGraphPowerPrivate()
{

}

IGraphPowerPrivate::~IGraphPowerPrivate()
{

}

IGraphPower::IGraphPower(QWidget *parent) : IGraph(*(new IGraphPowerPrivate),parent)
{

}
IGraphPower::~IGraphPower()
{

}

IGraphPower::IGraphPower(IGraphPowerPrivate &d, QWidget *parent):IGraph(d,parent)
{

}
