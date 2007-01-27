/***************************************************************************
                          DDLTrapezoid.cc  -  description
                             -------------------
    begin                : Mon Oct 29 2001
    email                : case@ucdhep.ucdavis.edu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *           DDDParser sub-component of DDD                                *
 *                                                                         *
 ***************************************************************************/



// -------------------------------------------------------------------------
// Includes 
// -------------------------------------------------------------------------
// Parser parts
#include "DetectorDescription/Parser/interface/DDLTrapezoid.h"
#include "DetectorDescription/Parser/interface/DDLElementRegistry.h"

// DDCore dependencies
#include "DetectorDescription/Core/interface/DDName.h"
#include "DetectorDescription/Core/interface/DDSolid.h"
#include "DetectorDescription/Base/interface/DDdebug.h"
#include "DetectorDescription/Base/interface/DDException.h"

#include "DetectorDescription/ExprAlgo/interface/ExprEvalSingleton.h"

#include <string>

// Default constructor
DDLTrapezoid::DDLTrapezoid()
{
}

// Default destructor
DDLTrapezoid::~DDLTrapezoid()
{
}

// Upon encountering an end of the tag, call DDCore's Trap.
void DDLTrapezoid::processElement (const std::string& name, const std::string& nmspace)
{
  DCOUT_V('P', "DDLTrapezoid::processElement started");

  ExprEvalInterface & ev = ExprEvalSingleton::instance();

  DDXMLAttribute atts = getAttributeSet();

  double phi = 0.0;
  double theta = 0.0;
  double dy2 = 0.0;

  if (atts.find("phi") != atts.end())
    phi = ev.eval(nmspace, atts.find("phi")->second);

  if (atts.find("theta") != atts.end())
    theta = ev.eval(nmspace, atts.find("theta")->second);

  if (atts.find("dy2") != atts.end())
    dy2 = ev.eval(nmspace, atts.find("dy2")->second);
  else if (atts.find("dy1") != atts.end())
    dy2 = ev.eval(nmspace, atts.find("dy1")->second);

  try {
    if (name == "Trapezoid")
      {
        DDSolid myTrap = 
	  DDSolidFactory::trap(getDDName(nmspace)
			     , ev.eval(nmspace, atts.find("dz")->second)
			     , theta
			     , phi
			     , ev.eval(nmspace, atts.find("h1")->second)
			     , ev.eval(nmspace, atts.find("bl1")->second)
			     , ev.eval(nmspace, atts.find("tl1")->second)
			     , ev.eval(nmspace, atts.find("alp1")->second)
			     , ev.eval(nmspace, atts.find("h2")->second)
			     , ev.eval(nmspace, atts.find("bl2")->second)
			     , ev.eval(nmspace, atts.find("tl2")->second)
			     , ev.eval(nmspace, atts.find("alp2")->second)
			     );

      }
    else if (name == "Trd1") 
      {
        DDSolid myTrd1 = 
	  DDSolidFactory::trap(getDDName(nmspace)
			     , ev.eval(nmspace, atts.find("dz")->second)
			     , 0
			     , 0
			     , ev.eval(nmspace, atts.find("dy1")->second)
			     , ev.eval(nmspace, atts.find("dx1")->second)
			     , ev.eval(nmspace, atts.find("dx1")->second)
			     , 0
			     , dy2
			     , ev.eval(nmspace, atts.find("dx2")->second)
			     , ev.eval(nmspace, atts.find("dx2")->second)
			     , 0
			     );
      }
    else
      {
        std::string msg = "\nDDLTrapezoid::processElement failed to process element of name: " 
	  + name
	  + ".  It can only process Trapezoid and Trd1.";
        throwError(msg);
      }
  } catch (DDException& e) {
    std::string msg = e.what();
    msg += std::string("\nDDLParser, Call failed to DDSolidFactory.");
    throwError(msg);
  }

  DDLSolid::setReference(nmspace);

  DCOUT_V('P', "DDLTrapezoid::processElement completed");
}
