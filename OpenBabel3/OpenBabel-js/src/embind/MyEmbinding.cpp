//#include <functional>
#include <emscripten/bind.h>

#include "openbabel/base.h"
//#include "openbabel/internalcoord.h"
#include "openbabel/atom.h"
#include "openbabel/bond.h"
#include "openbabel/mol.h"
#include "openbabel/residue.h"
#include "openbabel/reaction.h"
#include "openbabel/op.h"

#include "openbabel/obconversion.h"

#include "ObWrapper.cpp"

using namespace OpenBabel;
using namespace emscripten;

EMSCRIPTEN_BINDINGS(OB_GLOBAL) {
	emscripten::function("OBReleaseVersion", &OpenBabel::OBReleaseVersion);	

	register_vector<int>("vector<int>");
	register_vector<unsigned int>("vector<unsigned int>");
	register_vector<double>("vector<double>");
	register_vector<double*>("vector<double*>");
	register_vector<string>("vector<string>");
	register_vector<OBGenericData*>("vector<OBGenericData*>");
	register_vector<OBInternalCoord*>("vector<OBInternalCoord*>");
	register_vector<OBAtom*>("vector<OBAtom*>");
	register_vector<OBBond*>("vector<OBBond*>");
	register_vector<OBMol>("vector<OBMol>");

	register_map<std::string, std::string>("map<string, string>");
}


EMSCRIPTEN_BINDINGS(OBGenericData_Bind) {
    class_<OBGenericData>("OBGenericData")
        .constructor<std::string, unsigned int, DataOrigin>()
		.function("Clone", &OBGenericData::Clone, allow_raw_pointers())
		.function("SetAttribute", &OBGenericData::SetAttribute)
		.function("SetOrigin", &OBGenericData::SetOrigin)
		.function("GetAttribute", &OBGenericData::GetAttribute)
		.function("GetDataType", &OBGenericData::GetDataType)
		.function("GetValue", &OBGenericData::GetValue)
		.function("GetOrigin", &OBGenericData::GetOrigin)
		;
}
/*
EMSCRIPTEN_BINDINGS(OBPairData_Bind) {
    class_<OBPairData, base<OBGenericData>>("OBPairData")
        .constructor<>()
		.function("SetValue", select_overload<void(const std::string&)>(&OBPairData::SetValue))
		;
}
*/

EMSCRIPTEN_BINDINGS(OBBase_Bind) {
    class_<OBBase>("OBBase")
        .constructor<>()
        .function("Clear", &OBBase::Clear)
        .function("DoTransformations", &OBBase::DoTransformations, allow_raw_pointers())
		.class_function("ClassDescription", optional_override([](){
				string s = OBBase::ClassDescription();
				return s;
			}))		
		.function("GetTitle", optional_override([](OBBase& self){
				string s = self.GetTitle();
				return s;
			}))
		.function("SetTitle", optional_override([](OBBase& self, string s){
				self.SetTitle(s.c_str());
			}))

		.function("HasData", select_overload<bool(const std::string&)>(&OBBase::HasData), allow_raw_pointers())
		.function("HasData_Int", select_overload<bool(const unsigned int)>(&OBBase::HasData))
		.function("DeleteData", select_overload<bool(const std::string&)>(&OBBase::DeleteData))
		.function("DeleteData", select_overload<void(OBGenericData*)>(&OBBase::DeleteData), allow_raw_pointers())
		.function("DeleteData_Int", select_overload<void(const unsigned int)>(&OBBase::DeleteData))
		.function("DeleteData_Vector", select_overload<void(vector<OBGenericData*>&)>(&OBBase::DeleteData))
		.function("SetData", &OBBase::SetData, allow_raw_pointers())
		.function("CloneData", &OBBase::CloneData, allow_raw_pointers())
		.function("DataSize", &OBBase::DataSize)
		.function("GetData_Int", select_overload<OBGenericData*(const unsigned int)>(&OBBase::GetData), allow_raw_pointers())
		.function("GetData", select_overload<OBGenericData*(const std::string&)>(&OBBase::GetData), allow_raw_pointers())
		.function("GetAllData", &OBBase::GetAllData)
		;
}

EMSCRIPTEN_BINDINGS(ObBaseHelper_Bind) {
    class_<ObBaseHelper>("ObBaseHelper")
        .constructor<OBBase*>()
		.function("setOBBaseObj", &ObBaseHelper::setOBBaseObj, allow_raw_pointers())
		.function("getDataSize", &ObBaseHelper::getDataSize)
		.function("getDataAt", &ObBaseHelper::getDataAt, allow_raw_pointers())
		.function("getTitle", &ObBaseHelper::getTitle)
		.function("setTitle", &ObBaseHelper::setTitle)
		;
}


EMSCRIPTEN_BINDINGS(OBAtom_Bind) {
    class_<OBAtom, base<OBBase>>("OBAtom")
        .constructor<>()
        .function("Duplicate", &OBAtom::Duplicate, allow_raw_pointers())
		.function("Clear", &OBAtom::Clear)

		.function("SetIdx", &OBAtom::SetIdx)
		.function("SetId", &OBAtom::SetId)
		.function("SetHyb", &OBAtom::SetHyb)
		.function("SetAtomicNum", &OBAtom::SetAtomicNum)
		.function("SetIsotope", &OBAtom::SetIsotope)
		.function("SetImplicitHCount", &OBAtom::SetImplicitHCount)
		//.function("SetImplicitValence", &OBAtom::SetImplicitValence)
		//.function("IncrementImplicitValence", &OBAtom::IncrementImplicitValence)
		//.function("DecrementImplicitValence", &OBAtom::DecrementImplicitValence)
		.function("SetFormalCharge", &OBAtom::SetFormalCharge)
		.function("SetSpinMultiplicity", &OBAtom::SetSpinMultiplicity)
		.function("SetType", select_overload<void(const std::string&)>(&OBAtom::SetType))
				
		.function("SetPartialCharge", &OBAtom::SetPartialCharge)
		.function("SetVector", select_overload<void(double,double,double)>(&OBAtom::SetVector))
		.function("SetCoordPtr", &OBAtom::SetCoordPtr, allow_raw_pointers())
		.function("SetResidue", &OBAtom::SetResidue, allow_raw_pointers())
		.function("SetParent", &OBAtom::SetParent, allow_raw_pointers())
		.function("SetAromatic", &OBAtom::SetAromatic)
		// backward UnsetXXXX method
		.function("UnsetAromatic",optional_override([](OBAtom& self){ return self.SetAromatic(false); }))

		.function("SetInRing", &OBAtom::SetInRing)
		//.function("SetChiral", &OBAtom::SetChiral)
		.function("ClearCoordPtr", &OBAtom::ClearCoordPtr)

		.function("GetFormalCharge", &OBAtom::GetFormalCharge)
		.function("GetAtomicNum", &OBAtom::GetAtomicNum)
		.function("GetIsotope", &OBAtom::GetIsotope)
		.function("GetSpinMultiplicity", &OBAtom::GetSpinMultiplicity)
		.function("GetAtomicMass", &OBAtom::GetAtomicMass)
		.function("GetExactMass", &OBAtom::GetExactMass)
		.function("GetIdx", &OBAtom::GetIdx)
		.function("GetIndex", &OBAtom::GetIndex)
		.function("GetId", &OBAtom::GetId)
		.function("GetCoordinateIdx", &OBAtom::GetCoordinateIdx)
		//.function("GetValence", &OBAtom::GetValence)
		.function("GetExplicitDegree", &OBAtom::GetExplicitDegree)
		.function("GetTotalDegree", &OBAtom::GetTotalDegree)
		.function("GetExplicitValence", &OBAtom::GetExplicitValence)
		.function("GetTotalValence", &OBAtom::GetTotalValence)
		.function("GetHvyDegree", &OBAtom::GetHvyDegree)
		.function("GetHeteroDegree", &OBAtom::GetHeteroDegree)
		.function("GetHyb", &OBAtom::GetHyb)
		//.function("GetImplicitValence", &OBAtom::GetImplicitValence)
		//.function("GetHvyValence", &OBAtom::GetHvyValence)
		//.function("GetHeteroValence", &OBAtom::GetHeteroValence)
		//.function("GetType", &OBAtom::GetType, allow_raw_pointers())
		.function("GetImplicitHCount", &OBAtom::GetImplicitHCount)
		.function("GetX", &OBAtom::GetX)
		.function("GetY", &OBAtom::GetY)
		.function("GetZ", &OBAtom::GetZ)
		.function("x", &OBAtom::x)
		.function("y", &OBAtom::y)
		.function("z", &OBAtom::z)
		.function("GetCoordinate", &OBAtom::GetCoordinate, allow_raw_pointers())
		//.function("GetVector", select_overload<vector3(void)> &OBAtom::GetVector)

		.function("GetPartialCharge", &OBAtom::GetPartialCharge)
		//.function("GetResidue", select_overload<OBResidue*(bool)>(&OBAtom::GetResidue), allow_raw_pointers())
		.function("GetResidue", &OBAtom::GetResidue, allow_raw_pointers())
		.function("GetParent", &OBAtom::GetParent, allow_raw_pointers())
		.function("GetNewBondVector", &OBAtom::GetNewBondVector)
		.function("GetBond", &OBAtom::GetBond, allow_raw_pointers())
		//.function("GetNextAtom", &OBAtom::GetNextAtom, allow_raw_pointers())

		.function("GetDistance_Atom", select_overload<double(OBAtom*)>(&OBAtom::GetDistance), allow_raw_pointers())
		.function("GetDistance_Idx", select_overload<double(int)>(&OBAtom::GetDistance))
		.function("GetAngle_Atom", select_overload<double(OBAtom*, OBAtom*)>(&OBAtom::GetAngle), allow_raw_pointers())
		.function("GetAngle_Idx", select_overload<double(int, int)>(&OBAtom::GetAngle))

		.function("NewResidue", &OBAtom::NewResidue)
		.function("AddResidue", &OBAtom::AddResidue, allow_raw_pointers())
		.function("DeleteResidue", &OBAtom::DeleteResidue)

		.function("AddBond", &OBAtom::AddBond, allow_raw_pointers())
		.function("InsertBond", &OBAtom::InsertBond, allow_raw_pointers())
		.function("DeleteBond", &OBAtom::DeleteBond, allow_raw_pointers())
		.function("ClearBond", &OBAtom::ClearBond, allow_raw_pointers())

		.function("HtoMethyl", &OBAtom::NewResidue)
		.function("SetHybAndGeom", &OBAtom::SetHybAndGeom)
		//.function("ForceNoH", &OBAtom::ForceNoH)
		//.function("HasNoHForced", &OBAtom::HasNoHForced)
		//.function("ForceImplH", &OBAtom::ForceImplH)
		//.function("HasImplHForced", &OBAtom::HasImplHForced)

		.function("CountFreeOxygens", &OBAtom::CountFreeOxygens)
		//.function("ImplicitHydrogenCount", &OBAtom::ImplicitHydrogenCount)
		.function("ExplicitHydrogenCount", &OBAtom::ExplicitHydrogenCount)
		.function("MemberOfRingCount", &OBAtom::MemberOfRingCount)
		.function("MemberOfRingSize", &OBAtom::MemberOfRingSize)
		.function("CountRingBonds", &OBAtom::CountRingBonds)
		.function("SmallestBondAngle", &OBAtom::SmallestBondAngle)
		.function("AverageBondAngle", &OBAtom::AverageBondAngle)
		//.function("BOSum", &OBAtom::BOSum)
		.function("HasResidue", &OBAtom::HasResidue)
		.function("IsHetAtom", &OBAtom::IsHetAtom)
		.function("IsElement", &OBAtom::IsElement)

		//.function("IsHydrogen", select_overload<bool(void)>(&OBAtom::IsHydrogen))
		//.function("IsCarbon", &OBAtom::IsCarbon)
		//.function("IsNitrogen", &OBAtom::IsNitrogen)
		//.function("IsOxygen", &OBAtom::IsOxygen)
		//.function("IsSulfur", &OBAtom::IsSulfur)
		//.function("IsPhosphorus", &OBAtom::IsPhosphorus)
		.function("IsAromatic", &OBAtom::IsAromatic)
		.function("IsInRing", &OBAtom::IsInRing)
		.function("IsInRingSize", &OBAtom::IsInRingSize)
		.function("IsHeteroatom", &OBAtom::IsHeteroatom)
		//.function("IsNotCorH", &OBAtom::IsNotCorH)
		.function("IsConnected", &OBAtom::IsConnected, allow_raw_pointers())
		.function("IsOneThree", &OBAtom::IsOneThree, allow_raw_pointers())
		.function("IsOneFour", &OBAtom::IsOneFour, allow_raw_pointers())
		.function("IsCarboxylOxygen", &OBAtom::IsCarboxylOxygen)
		.function("IsPhosphateOxygen", &OBAtom::IsPhosphateOxygen)
		.function("IsSulfateOxygen", &OBAtom::IsSulfateOxygen)
		.function("IsNitroOxygen", &OBAtom::IsNitroOxygen)
		.function("IsAmideNitrogen", &OBAtom::IsAmideNitrogen)
		.function("IsPolarHydrogen", &OBAtom::IsPolarHydrogen)
		.function("IsNonPolarHydrogen", &OBAtom::IsNonPolarHydrogen)
		.function("IsAromaticNOxide", &OBAtom::IsAromaticNOxide)
		.function("IsChiral", &OBAtom::IsChiral)
		.function("IsAxial", &OBAtom::IsAxial)
		.function("IsHbondAcceptor", &OBAtom::IsHbondAcceptor)
		.function("IsHbondAcceptorSimple", &OBAtom::IsHbondAcceptorSimple)
		.function("IsHbondDonor", &OBAtom::IsHbondDonor)
		.function("IsHbondDonorH", &OBAtom::IsHbondDonorH)
		.function("IsMetal", &OBAtom::IsMetal)

		.function("HasAlphaBetaUnsat", &OBAtom::HasAlphaBetaUnsat)
		.function("HasBondOfOrder", &OBAtom::HasBondOfOrder)
		.function("CountBondsOfOrder", &OBAtom::CountBondsOfOrder)
		.function("HighestBondOrder", &OBAtom::HighestBondOrder)
		.function("HasNonSingleBond", &OBAtom::HasNonSingleBond)
		.function("HasSingleBond", &OBAtom::HasSingleBond)
		.function("HasDoubleBond", &OBAtom::HasDoubleBond)
		.function("HasAromaticBond", &OBAtom::HasAromaticBond)
		.function("MatchesSMARTS", &OBAtom::MatchesSMARTS, allow_raw_pointers())
        ;
}

EMSCRIPTEN_BINDINGS(OBBond_Bind) {
    class_<OBBond, base<OBBase>>("OBBond")
		.constructor<>()        
		.function("SetIdx", &OBBond::SetIdx)
		.function("SetId", &OBBond::SetId)
		.function("SetBondOrder", &OBBond::SetBondOrder)
		.function("SetBegin", &OBBond::SetBegin, allow_raw_pointers())
		.function("SetEnd", &OBBond::SetEnd, allow_raw_pointers())
		.function("SetParent", &OBBond::SetParent, allow_raw_pointers())
		.function("SetLength_Fix", select_overload<void(OBAtom*, double)>(&OBBond::SetLength), allow_raw_pointers())
		.function("SetLength", select_overload<void(double)>(&OBBond::SetLength))
		.function("Set", &OBBond::Set, allow_raw_pointers())
		.function("SetAromatic", &OBBond::SetAromatic)
		.function("SetWedge", &OBBond::SetWedge)
		.function("SetHash", &OBBond::SetHash)
		.function("SetWedgeOrHash", &OBBond::SetWedgeOrHash)
		//.function("SetUp", &OBBond::SetUp)
		//.function("SetDown", &OBBond::SetDown)
		.function("SetInRing", &OBBond::SetInRing)
		.function("SetClosure", &OBBond::SetClosure)

		// backward unsetXXXX methods
		.function("UnsetHash",optional_override([](OBBond& self){ return self.SetHash(false); }))
		.function("UnsetWedge",optional_override([](OBBond& self){ return self.SetWedge(false); }))
		.function("UnsetUp",optional_override([](OBBond& self){ return self.SetWedge(false); }))
		.function("UnsetDown",optional_override([](OBBond& self){ return self.SetHash(false); }))
		.function("UnsetAromatic",optional_override([](OBBond& self){ return self.SetAromatic(false); }))		
		//.function("UnsetHash", &OBBond::UnsetHash)
		//.function("UnsetWedge", &OBBond::UnsetWedge)
		//.function("UnsetUp", &OBBond::UnsetUp)
		//.function("UnsetDown", &OBBond::UnsetDown)
		//.function("UnsetAromatic", &OBBond::UnsetAromatic)
		//.function("UnsetKekule", &OBBond::UnsetKekule)

		.function("GetIdx", &OBBond::GetIdx)
		.function("GetId", &OBBond::GetId)
		.function("GetBondOrder", &OBBond::GetBondOrder)
		.function("GetFlags", &OBBond::GetFlags)
		.function("GetBeginAtomIdx", &OBBond::GetBeginAtomIdx)
		.function("GetEndAtomIdx", &OBBond::GetEndAtomIdx)
		.function("GetBeginAtom", select_overload<OBAtom*(void)>(&OBBond::GetBeginAtom), allow_raw_pointers())
		.function("GetEndAtom", select_overload<OBAtom*(void)>(&OBBond::GetEndAtom), allow_raw_pointers())
		.function("GetNbrAtom", &OBBond::GetNbrAtom, allow_raw_pointers())
		.function("GetParent", &OBBond::GetParent, allow_raw_pointers())

		.function("GetEquibLength", &OBBond::GetEquibLength)
		.function("GetLength", &OBBond::GetLength)
		.function("GetNbrAtomIdx", &OBBond::GetNbrAtomIdx, allow_raw_pointers())
		.function("FindSmallestRing", &OBBond::FindSmallestRing, allow_raw_pointers())

		.function("IsAromatic", &OBBond::IsAromatic)
		.function("IsInRing", &OBBond::IsInRing)
		.function("IsRotor", &OBBond::IsRotor)
		.function("IsAmide", &OBBond::IsAmide)
		.function("IsPrimaryAmide", &OBBond::IsPrimaryAmide)
		.function("IsSecondaryAmide", &OBBond::IsSecondaryAmide)
		.function("IsTertiaryAmide", &OBBond::IsTertiaryAmide)
		.function("IsEster", &OBBond::IsEster)
		.function("IsCarbonyl", &OBBond::IsCarbonyl)
		//.function("IsSingle", &OBBond::IsSingle)
		//.function("IsDouble", &OBBond::IsDouble)
		//.function("IsTriple", &OBBond::IsTriple)

		.function("IsClosure", &OBBond::IsClosure)
		//.function("IsUp", &OBBond::IsUp)
		//.function("IsDown", &OBBond::IsDown)
		.function("IsWedge", &OBBond::IsWedge)
		.function("IsHash", &OBBond::IsHash)
		.function("IsWedgeOrHash", &OBBond::IsWedgeOrHash)
		.function("IsCisOrTrans", &OBBond::IsCisOrTrans)
		.function("IsDoubleBondGeometry", &OBBond::IsDoubleBondGeometry)

		;

	enum_<OBBond::Flag>("OBBond_Flag")
        .value("Aromatic", OBBond::Aromatic)
        .value("Ring", OBBond::Ring)
		.value("Closure", OBBond::Closure)
        ;
	enum_<OBBond::StereoFlag>("OBBond_StereoFlag")
        .value("Wedge", OBBond::Wedge)
        .value("Hash", OBBond::Hash)
		.value("WedgeOrHash", OBBond::WedgeOrHash)
		.value("CisOrTrans", OBBond::CisOrTrans)
        ;
}


EMSCRIPTEN_BINDINGS(OBMol_Bind) {
    class_<OBMol, base<OBBase>>("OBMol")
        .constructor<>()
		//.smart_ptr<std::shared_ptr<OBMol>>()
		// .smart_ptr_constructor(&std::make_shared<OBMol>)
		.function("ReserveAtoms", &OBMol::ReserveAtoms)
		///.function("DestroyAtom", &OBMol::DestroyAtom)
		///.function("DestroyBond", &OBMol::DestroyBond)
		///.function("DestroyResidue", &OBMol::DestroyResidue)

		.function("AddAtom", &OBMol::AddAtom)
		.function("InsertAtom", &OBMol::InsertAtom)
		.function("AddBond", select_overload<bool(OBBond&)>(&OBMol::AddBond))
		.function("AddBondWithParam", select_overload<bool(int, int, int, int, int)>(&OBMol::AddBond))
		.function("AddResidue", &OBMol::AddResidue)

		.function("NewAtom", select_overload<OBAtom*(void)>(&OBMol::NewAtom), allow_raw_pointers())
		.function("NewAtomWithId", select_overload<OBAtom*(unsigned long)>(&OBMol::NewAtom), allow_raw_pointers())
		.function("NewBond", select_overload<OBBond*(void)>(&OBMol::NewBond), allow_raw_pointers())
		.function("NewBondWithId", select_overload<OBBond*(unsigned long)>(&OBMol::NewBond), allow_raw_pointers())
		.function("NewResidue", &OBMol::NewResidue, allow_raw_pointers())
		.function("DeleteAtom", &OBMol::DeleteAtom, allow_raw_pointers())
		.function("DeleteBond", &OBMol::DeleteBond, allow_raw_pointers())
		.function("DeleteResidue", &OBMol::DeleteResidue, allow_raw_pointers())

		.function("BeginModify", &OBMol::BeginModify)
		.function("EndModify", &OBMol::EndModify)

		.function("GetFlags", &OBMol::GetFlags)
		.function("GetTitle", &OBMol::GetTitle, allow_raw_pointers())
		.function("NumAtoms", &OBMol::NumAtoms)
		.function("NumBonds", &OBMol::NumBonds)
		.function("NumHvyAtoms", &OBMol::NumHvyAtoms)
		.function("NumResidues", &OBMol::NumResidues)
		.function("NumRotors", &OBMol::NumRotors)

		.function("GetAtom", &OBMol::GetAtom, allow_raw_pointers())
		.function("GetAtomById", &OBMol::GetAtomById, allow_raw_pointers())
		.function("GetFirstAtom", &OBMol::GetFirstAtom, allow_raw_pointers())
		.function("GetBondById", &OBMol::GetBondById, allow_raw_pointers())
		.function("GetBond", select_overload<OBBond*(int) const>(&OBMol::GetBond), allow_raw_pointers())
		.function("GetBondByAtomIndexes", select_overload<OBBond*(int, int) const>(&OBMol::GetBond), allow_raw_pointers())
		.function("GetBondByAtoms", select_overload<OBBond*(OBAtom*, OBAtom*) const>(&OBMol::GetBond), allow_raw_pointers())
		.function("GetResidue", &OBMol::GetResidue, allow_raw_pointers())

		//.function("GetInternalCoord", &OBMol::GetInternalCoord)
		.function("GetTorsion", select_overload<double(OBAtom*, OBAtom*, OBAtom*, OBAtom*)>(&OBMol::GetTorsion), allow_raw_pointers())
		.function("GetTorsionByAtomIndexes", select_overload<double(int, int, int, int)>(&OBMol::GetTorsion))
		.function("GetAngle", &OBMol::GetAngle, allow_raw_pointers())
		.function("AreInSameRing", &OBMol::AreInSameRing, allow_raw_pointers())
		.function("GetFormula", &OBMol::GetFormula)
		.function("GetSpacedFormula", &OBMol::GetSpacedFormula, allow_raw_pointers())
		.function("GetEnergy", select_overload<double(void) const>(&OBMol::GetEnergy))
		.function("GetMolWt", &OBMol::GetMolWt)
		.function("GetExactMass", &OBMol::GetExactMass)
		.function("GetTotalCharge", &OBMol::GetTotalCharge)
		.function("GetTotalSpinMultiplicity", &OBMol::GetTotalSpinMultiplicity)
		.function("GetDimension", &OBMol::GetDimension)
		.function("GetCoordinates", &OBMol::GetCoordinates, allow_raw_pointers())
		.function("GetSSSR", &OBMol::GetSSSR)
		.function("GetLSSR", &OBMol::GetLSSR)
		.function("AutomaticFormalCharge", &OBMol::AutomaticFormalCharge)
		.function("AutomaticPartialCharge", &OBMol::AutomaticPartialCharge)

		.function("SetTitle", select_overload<void(const char*)>(&OBMol::SetTitle), allow_raw_pointers())  //???
		.function("SetFormula", &OBMol::SetFormula)
		.function("SetEnergy", &OBMol::SetEnergy)
		.function("SetDimension", &OBMol::SetDimension)
		.function("SetTotalCharge", &OBMol::SetTotalCharge)
		.function("SetTotalSpinMultiplicity", &OBMol::SetTotalSpinMultiplicity)
		//.function("SetInternalCoord", &OBMol::SetInternalCoord)
		.function("SetAutomaticFormalCharge", &OBMol::SetAutomaticFormalCharge)
		.function("SetAutomaticPartialCharge", &OBMol::SetAutomaticPartialCharge)

		.function("SetAromaticPerceived", &OBMol::SetAromaticPerceived)
		.function("SetSSSRPerceived", &OBMol::SetSSSRPerceived)
		.function("SetLSSRPerceived", &OBMol::SetLSSRPerceived)
		.function("SetRingAtomsAndBondsPerceived", &OBMol::SetRingAtomsAndBondsPerceived)
		.function("SetAtomTypesPerceived", &OBMol::SetAtomTypesPerceived)
		.function("SetRingTypesPerceived", &OBMol::SetRingTypesPerceived)
		.function("SetChainsPerceived", &OBMol::SetChainsPerceived)
		.function("SetChiralityPerceived", &OBMol::SetChiralityPerceived)
		.function("SetPartialChargesPerceived", &OBMol::SetPartialChargesPerceived)
		.function("SetHybridizationPerceived", &OBMol::SetHybridizationPerceived)
		//.function("SetImplicitValencePerceived", &OBMol::SetImplicitValencePerceived)
		//.function("SetKekulePerceived", &OBMol::SetKekulePerceived)
		.function("SetClosureBondsPerceived", &OBMol::SetClosureBondsPerceived)
		.function("SetHydrogensAdded", &OBMol::SetHydrogensAdded)
		.function("SetCorrectedForPH", &OBMol::SetCorrectedForPH)
		//.function("SetAromaticCorrected", &OBMol::SetAromaticCorrected)
		.function("SetSpinMultiplicityAssigned", &OBMol::SetSpinMultiplicityAssigned)
		.function("SetIsPatternStructure", &OBMol::SetIsPatternStructure)
		.function("SetIsReaction", &OBMol::SetIsReaction)
		.function("SetPeriodicMol", &OBMol::SetPeriodicMol)

		.function("HasFlag", &OBMol::HasFlag)
		.function("SetFlag", &OBMol::SetFlag)
		.function("UnsetFlag", &OBMol::UnsetFlag)
		.function("SetFlags", &OBMol::SetFlags)

		//.function("UnsetAromaticPerceived", &OBMol::UnsetAromaticPerceived)
		//.function("UnsetSSSRPerceived", &OBMol::UnsetSSSRPerceived)
		//.function("UnsetRingTypesPerceived", &OBMol::UnsetRingTypesPerceived)
		//.function("UnsetPartialChargesPerceived", &OBMol::UnsetPartialChargesPerceived)
		//.function("UnsetImplicitValencePerceived", &OBMol::UnsetImplicitValencePerceived)
		//.function("UnsetHydrogensAdded", &OBMol::UnsetHydrogensAdded)
		//.function("UnsetFlag", &OBMol::UnsetFlag)

		.function("DoTransformations", &OBMol::DoTransformations, allow_raw_pointers())
		.function("Clear", &OBMol::Clear)
		.function("RenumberAtoms", select_overload<void(std::vector<OBAtom*>&)>(&OBMol::RenumberAtoms), allow_raw_pointers())
		.function("RenumberAtoms_Indexes", select_overload<void(std::vector<int>)>(&OBMol::RenumberAtoms))
		.function("SetCoordinates", &OBMol::SetCoordinates, allow_raw_pointers())
		.function("ToInertialFrameConformer", select_overload<void(int, double*)>(&OBMol::ToInertialFrame), allow_raw_pointers())
		.function("ToInertialFrame", select_overload<void(void)>(&OBMol::ToInertialFrame))
		.function("Translate", select_overload<void(const vector3 &)>(&OBMol::Translate))
		.function("TranslateConformer", select_overload<void(const vector3 &, int)>(&OBMol::Translate))
		.function("Rotate", select_overload<void(const double [9])>(&OBMol::Rotate), allow_raw_pointers())
		.function("RotateConformer", select_overload<void(const double [9], int)>(&OBMol::Rotate), allow_raw_pointers())
		.function("Center", select_overload<void(void)>(&OBMol::Center))

		//.function("Kekulize", &OBMol::Kekulize)
		//.function("PerceiveKekuleBonds", &OBMol::PerceiveKekuleBonds)

		//.function("NewPerceiveKekuleBonds", &OBMol::NewPerceiveKekuleBonds)

		.function("DeleteHydrogens", select_overload<bool(void)>(&OBMol::DeleteHydrogens))
		.function("DeleteHydrogensOfAtom", select_overload<bool(OBAtom*)>(&OBMol::DeleteHydrogens), allow_raw_pointers())
		.function("DeleteNonPolarHydrogens", &OBMol::DeleteNonPolarHydrogens)
		.function("DeleteHydrogen", &OBMol::DeleteHydrogen, allow_raw_pointers())
		.function("AddHydrogens", select_overload<bool(OBAtom*)>(&OBMol::AddHydrogens), allow_raw_pointers())
		.function("AddHydrogensWithParam", select_overload<bool(bool, bool, double)>(&OBMol::AddHydrogens))
		.function("AddPolarHydrogens", &OBMol::AddPolarHydrogens)
		.function("AddNonPolarHydrogens", &OBMol::AddNonPolarHydrogens)
		.function("AddNewHydrogens", &OBMol::AddNewHydrogens)

		.function("StripSalts", &OBMol::StripSalts)
		.function("Separate", &OBMol::Separate)
		///.function("GetNextFragment", &OBMol::GetNextFragment)
		///.function("CopySubstructure", &OBMol::CopySubstructure)
		.function("ConvertDativeBonds", &OBMol::ConvertDativeBonds)
		.function("MakeDativeBonds", &OBMol::MakeDativeBonds)
		.function("ConvertZeroBonds", &OBMol::ConvertZeroBonds)

		.function("CorrectForPH", &OBMol::CorrectForPH)
		.function("AssignSpinMultiplicity", &OBMol::AssignSpinMultiplicity)
		.function("SetIsPatternStructure", &OBMol::SetIsPatternStructure)

		.function("CenterConformer", select_overload<vector3(int)>(&OBMol::Center))

		.function("SetTorsion", &OBMol::SetTorsion, allow_raw_pointers())
		.function("FindSSSR", &OBMol::FindSSSR)
		.function("FindLSSR", &OBMol::FindLSSR)
		.function("FindRingAtomsAndBonds", &OBMol::FindRingAtomsAndBonds)
		//.function("FindChiralCenters", &OBMol::FindChiralCenters)
		.function("FindChildren", select_overload<void(vector<OBAtom*>&, OBAtom*, OBAtom*)>(&OBMol::FindChildren), allow_raw_pointers())
		.function("FindChildrenByIndexes", select_overload<void(std::vector<int>&, int, int)>(&OBMol::FindChildren), allow_raw_pointers())
		.function("FindLargestFragment", &OBMol::FindLargestFragment)
		.function("ContigFragList", &OBMol::ContigFragList)
		.function("Align", &OBMol::Align, allow_raw_pointers())
		.function("ConnectTheDots", &OBMol::ConnectTheDots)
		.function("PerceiveBondOrders", &OBMol::PerceiveBondOrders)
		.function("FindAngles", &OBMol::FindAngles)
		.function("FindTorsions", &OBMol::FindTorsions)
		.function("GetGTDVector", &OBMol::GetGTDVector)
		.function("GetGIVector", &OBMol::GetGIVector)
		.function("GetGIDVector", &OBMol::GetGIDVector)
		.function("GetGIVector", &OBMol::GetGIVector)

		.function("Has2D", &OBMol::Has2D)
		.function("Has3D", &OBMol::Has3D)
		.function("HasNonZeroCoords", &OBMol::HasNonZeroCoords)
		.function("HasAromaticPerceived", &OBMol::HasAromaticPerceived)
		.function("HasSSSRPerceived", &OBMol::HasSSSRPerceived)
		.function("HasLSSRPerceived", &OBMol::HasLSSRPerceived)
		.function("HasRingAtomsAndBondsPerceived", &OBMol::HasRingAtomsAndBondsPerceived)
		.function("HasAtomTypesPerceived", &OBMol::HasAtomTypesPerceived)
		.function("HasRingTypesPerceived", &OBMol::HasRingTypesPerceived)
		.function("HasChiralityPerceived", &OBMol::HasChiralityPerceived)
		.function("HasPartialChargesPerceived", &OBMol::HasPartialChargesPerceived)
		.function("HasHybridizationPerceived", &OBMol::HasHybridizationPerceived)
		//.function("HasImplicitValencePerceived", &OBMol::HasImplicitValencePerceived)
		//.function("HasKekulePerceived", &OBMol::HasKekulePerceived)
		.function("HasClosureBondsPerceived", &OBMol::HasClosureBondsPerceived)
		.function("HasChainsPerceived", &OBMol::HasChainsPerceived)
		.function("HasHydrogensAdded", &OBMol::HasHydrogensAdded)
		//.function("HasAromaticCorrected", &OBMol::HasAromaticCorrected)
		.function("IsCorrectedForPH", &OBMol::IsCorrectedForPH)
		.function("HasSpinMultiplicityAssigned", &OBMol::HasSpinMultiplicityAssigned)
		//.function("IsChiral", &OBMol::IsChiral)
		.function("IsReaction", &OBMol::IsReaction)
		.function("IsPeriodic", &OBMol::IsPeriodic)
		.function("Empty", &OBMol::Empty)

		.function("NumConformers", &OBMol::NumConformers)
		.function("SetConformers", &OBMol::SetConformers, allow_raw_pointers())
		.function("AddConformer", &OBMol::AddConformer, allow_raw_pointers())
		.function("SetConformer", &OBMol::SetConformer)
		.function("CopyConformer", &OBMol::CopyConformer, allow_raw_pointers())
		.function("DeleteConformer", &OBMol::DeleteConformer)
		.function("GetConformer", &OBMol::GetConformer, allow_raw_pointers())
		.function("SetEnergies", &OBMol::SetEnergies)
		.function("GetEnergies", &OBMol::GetEnergies)
		.function("GetEnergy", select_overload<double(int)>(&OBMol::GetEnergy))		
        ;
}

EMSCRIPTEN_BINDINGS(OBResidue_Bind) {
    class_<OBResidue, base<OBBase>>("OBResidue")
        .constructor<>()
		.function("AddAtom", &OBResidue::AddAtom, allow_raw_pointers())		
		.function("InsertAtom", &OBResidue::InsertAtom, allow_raw_pointers())		
		.function("RemoveAtom", &OBResidue::RemoveAtom, allow_raw_pointers())
		.function("Clear", &OBResidue::Clear)

		.function("SetName", &OBResidue::SetName)
		.function("SetNum", select_overload<void(const unsigned int)>(&OBResidue::SetNum))
		.function("SetNumStr", select_overload<void(const string)>(&OBResidue::SetNum))
		.function("SetChain", &OBResidue::SetChain)
		.function("SetChainNum", &OBResidue::SetChainNum)
		.function("SetIdx", &OBResidue::SetIdx)
		.function("SetInsertionCode", &OBResidue::SetInsertionCode)
		.function("SetAtomID", &OBResidue::SetAtomID, allow_raw_pointers())
		.function("SetHetAtom", &OBResidue::SetHetAtom, allow_raw_pointers())
		.function("SetSerialNum", &OBResidue::SetSerialNum, allow_raw_pointers())

		.function("GetName", &OBResidue::GetName)
		.function("GetNum", &OBResidue::GetNum)
		.function("GetNumString", &OBResidue::GetNumString)
		.function("GetNumAtoms", &OBResidue::GetNumAtoms)
		.function("GetChain", &OBResidue::GetChain)
		.function("GetChainNum", &OBResidue::GetChainNum)
		.function("GetIdx", &OBResidue::GetIdx)
		.function("GetResKey", &OBResidue::GetResKey)

		.function("GetAtoms", &OBResidue::GetAtoms)
		.function("GetBonds", &OBResidue::GetBonds)

		.function("GetAtomID", &OBResidue::GetAtomID, allow_raw_pointers())
		.function("GetSerialNum", &OBResidue::GetSerialNum, allow_raw_pointers())
		.function("GetInsertionCode", &OBResidue::GetInsertionCode)
		.function("GetAminoAcidProperty", &OBResidue::GetAminoAcidProperty)
		.function("GetAtomProperty", &OBResidue::GetAtomProperty, allow_raw_pointers())
		.function("GetResidueProperty", &OBResidue::GetResidueProperty)
		.function("IsHetAtom", &OBResidue::IsHetAtom, allow_raw_pointers())
		.function("IsResidueType", &OBResidue::IsResidueType)
		;

	/*	
	enum_<OBResidueAtomProperty::>("OBResidueAtomProperty")
        .value("ALPHA_CARBON", OBResidueAtomProperty::ALPHA_CARBON)
        .value("AMINO_BACKBONE", OBResidueAtomProperty::AMINO_BACKBONE)
		.value("BACKBONE", OBResidueAtomProperty::BACKBONE)
		.value("CYSTEINE_SULPHUR", OBResidueAtomProperty::CYSTEINE_SULPHUR)
		.value("LIGAND", OBResidueAtomProperty::LIGAND)
		.value("NUCLEIC_BACKBONE", OBResidueAtomProperty::NUCLEIC_BACKBONE)
		.value("SHAPELY_BACKBONE", OBResidueAtomProperty::SHAPELY_BACKBONE)
		.value("SHAPELY_SPECIAL", OBResidueAtomProperty::SHAPELY_SPECIAL)
		.value("SIDECHAIN", OBResidueAtomProperty::SIDECHAIN)
		.value("SUGAR_PHOSPHATE", OBResidueAtomProperty::SUGAR_PHOSPHATE)
        ;
    */    
}


EMSCRIPTEN_BINDINGS(OBReaction_Bind) {
    class_<OBReaction, base<OBBase>>("OBReaction")
        .constructor<>()
		.function("NumReactants", &OBReaction::NumReactants)
		.function("NumProducts", &OBReaction::NumProducts)
		.function("AddReactant", &OBReaction::AddReactant)
		.function("AddProduct", &OBReaction::AddProduct)
		.function("SetTransitionState", &OBReaction::SetTransitionState)
		.function("AddAgent", &OBReaction::AddAgent)

		.function("GetReactant", &OBReaction::GetReactant)
		.function("GetProduct", &OBReaction::GetProduct)
		.function("GetTransitionState", &OBReaction::GetTransitionState)
		.function("GetAgent", &OBReaction::GetAgent)

		.function("GetTitle", &OBReaction::GetTitle)
		.function("GetComment", &OBReaction::GetComment)
		.function("SetTitle", &OBReaction::SetTitle)
		.function("SetComment", &OBReaction::SetComment)

		.function("IsReversible", &OBReaction::IsReversible)
		.function("SetReversible", &OBReaction::SetReversible)

		.function("Clear", &OBReaction::Clear)
		;
}


EMSCRIPTEN_BINDINGS(OBFormat_Bind) {
    class_<OBFormat, base<OBPlugin>>("OBFormat")
        //.constructor<>()		
		.class_function("FindType", optional_override([](const std::string id){
				return OBFormat::FindType(id.c_str());
			}), allow_raw_pointers())
		.class_function("FormatFromMIME", optional_override([](const std::string MIME){
				return OBFormat::FormatFromMIME(MIME.c_str());
			}), allow_raw_pointers())
		.function("TypeID", optional_override([](OBFormat& self){
				std::string s = self.TypeID();
				return s;
			}))
		.function("ReadMolecule", &OBFormat::ReadMolecule, allow_raw_pointers())
		.function("ReadChemObject", &OBFormat::ReadChemObject, allow_raw_pointers())
		.function("WriteMolecule", &OBFormat::WriteMolecule, allow_raw_pointers())
		.function("WriteChemObject", &OBFormat::WriteChemObject, allow_raw_pointers())
		//.function("Description", &OBFormat::Description, allow_raw_pointers())
		.function("Description", optional_override([](OBFormat& self){
				std::string s = self.Description();
				return s;
			}))
		//.function("TargetClassDescription", &OBFormat::TargetClassDescription, allow_raw_pointers())
		.function("TargetClassDescription", optional_override([](OBFormat& self){
				std::string s = self.TargetClassDescription();
				return s;
			}))		
		.function("SpecificationURL", optional_override([](OBFormat& self){
				std::string s = self.SpecificationURL();
				return s;
			}))
		.function("GetMIMEType", optional_override([](OBFormat& self){
				std::string s = self.GetMIMEType();
				return s;
			}))
		.function("Flags", &OBFormat::Flags)
		.function("SkipObjects", &OBFormat::SkipObjects, allow_raw_pointers())
		.function("MakeNewInstance", &OBFormat::MakeNewInstance, allow_raw_pointers())
		.function("RegisterFormat", optional_override([](OBFormat& self, string ID, string MIME = ""){
				return self.RegisterFormat(ID.c_str(), MIME.c_str());
			}))
		//.function("Display", &OBFormat::Display, allow_raw_pointers())
		.function("Display", optional_override([](OBFormat& self, string txt, string param, string ID = ""){
				return self.Display(txt, param.c_str(), ID.c_str());
			}))
		;
}


EMSCRIPTEN_BINDINGS(ObFormatInfo_Bind) {
    value_object<ObFormatInfo>("ObFormatInfo")
        .field("id", &ObFormatInfo::id)
		.field("mimeType", &ObFormatInfo::mimeType)
		.field("description", &ObFormatInfo::description)
		.field("specificationURL", &ObFormatInfo::specificationURL)
        ;
}

EMSCRIPTEN_BINDINGS(ObFormatWrapper_Bind) {
    class_<ObFormatWrapper>("ObFormatWrapper")
        .constructor<>()
		.function("getId", &ObFormatWrapper::getId)
		.function("getDescription", &ObFormatWrapper::getDescription)
		.function("getTargetClassDescription", &ObFormatWrapper::getTargetClassDescription)
		.function("getSpecificationURL", &ObFormatWrapper::getSpecificationURL)
		.function("getMIMEType", &ObFormatWrapper::getMIMEType)
		.function("getTypeName", &ObFormatWrapper::getTypeName)
		;
}

EMSCRIPTEN_BINDINGS(OBConversion_Bind) {
    class_<OBConversion>("OBConversion")
        .constructor<>(optional_override([](){
			return new OBConversion();
		}), allow_raw_pointers())
		.class_function("RegisterFormat", optional_override([](string ID, OBFormat* pFormat, string MIME = ""){
				return OBConversion::RegisterFormat(ID.c_str(), pFormat, MIME.c_str());
			}), allow_raw_pointers())
		.class_function("FindFormat", select_overload<OBFormat*(string)>(&OBConversion::FindFormat), allow_raw_pointers())
		.class_function("FormatFromExt", select_overload<OBFormat*(string)>(&OBConversion::FormatFromExt), allow_raw_pointers())
		//.class_function("FormatFromExt2", select_overload<OBFormat*(string, bool&)>(&OBConversion::FormatFromExt), allow_raw_pointers())
		.class_function("FormatFromMIME", optional_override([](string MIME){
				return OBConversion::FormatFromMIME(MIME.c_str());
			}), allow_raw_pointers())
		.class_function("Description", optional_override([](){
				std::string s = OBConversion::Description();
				return s;
			}))
		.function("SetInAndOutFormats", select_overload<bool(OBFormat*, OBFormat*, bool, bool)>(&OBConversion::SetInAndOutFormats), allow_raw_pointers())
		.function("SetInAndOutFormatIDs", optional_override([](OBConversion& self, string inID, string outID, bool ingzip=false, bool outgzip=false){
				return self.SetInAndOutFormats(inID.c_str(), outID.c_str(), ingzip, outgzip);
			}), allow_raw_pointers())
		.function("SetInFormat", select_overload<bool(OBFormat*, bool)>(&OBConversion::SetInFormat), allow_raw_pointers())
		.function("SetInFormatID", optional_override([](OBConversion& self, string ID, bool isgzip=false){
				return self.SetInFormat(ID.c_str(), isgzip);
			}))
		.function("SetOutFormat", select_overload<bool(OBFormat*, bool)>(&OBConversion::SetOutFormat), allow_raw_pointers())
		.function("SetOutFormatID", optional_override([](OBConversion& self, string ID, bool isgzip=false){
				return self.SetOutFormat(ID.c_str(), isgzip);
			}))
		.function("GetInFormat", &OBConversion::GetInFormat, allow_raw_pointers())
		.function("GetOutFormat", &OBConversion::GetOutFormat, allow_raw_pointers())
		.function("GetInGzipped", &OBConversion::GetOutFormat, allow_raw_pointers())
		.function("GetOutGzipped", &OBConversion::GetOutFormat, allow_raw_pointers())

		.function("GetInLen", &OBConversion::GetInLen, allow_raw_pointers())
		.function("GetTitle", optional_override([](OBConversion& self){
				string s = self.GetTitle();
				return s;
			}))
		.function("GetAuxConv", &OBConversion::GetAuxConv, allow_raw_pointers())
		.function("SetAuxConv", &OBConversion::SetAuxConv, allow_raw_pointers())

		.function("IsOption", optional_override([](OBConversion& self, string opt, OBConversion::Option_type opttyp=OBConversion::Option_type::OUTOPTIONS){
				string s = self.IsOption(opt.c_str(), opttyp);
				return s;
			}))

		.function("GetOptions", &OBConversion::GetOptions, allow_raw_pointers())
		/*
		.function("GetOptions", optional_override([](OBConversion& self, OBConversion::Option_type opttyp=OBConversion::Option_type::OUTOPTIONS){
				return *(self.GetOptions(opttyp));
			}))
		*/
 
		.function("AddOption", optional_override([](OBConversion& self, string opt, OBConversion::Option_type opttyp=OBConversion::Option_type::OUTOPTIONS, string txt=""){				
				return self.AddOption(opt.c_str(), opttyp, txt.c_str());
			}))
		.function("RemoveOption", optional_override([](OBConversion& self, string opt, OBConversion::Option_type opttyp=OBConversion::Option_type::OUTOPTIONS){				
				return self.RemoveOption(opt.c_str(), opttyp);
			}))
		.function("SetOptions", optional_override([](OBConversion& self, string options, OBConversion::Option_type opttyp=OBConversion::Option_type::OUTOPTIONS){				
				return self.RemoveOption(options.c_str(), opttyp);
			}))
		.class_function("RegisterOptionParam", &OBConversion::RegisterOptionParam, allow_raw_pointers())
		.class_function("GetOptionParams", &OBConversion::GetOptionParams, allow_raw_pointers())
		.function("CopyOptions", &OBConversion::CopyOptions, allow_raw_pointers())

		.function("GetSupportedInputFormat", &OBConversion::GetSupportedInputFormat, allow_raw_pointers())
		.function("GetSupportedOutputFormat", &OBConversion::GetSupportedOutputFormat, allow_raw_pointers())
		.class_function("GetDefaultFormat", &OBConversion::GetDefaultFormat, allow_raw_pointers())

		.function("Convert", select_overload<int()>(&OBConversion::Convert))

		.function("AddChemObject", &OBConversion::AddChemObject, allow_raw_pointers())
		.function("GetChemObject", &OBConversion::GetChemObject, allow_raw_pointers())
		.function("IsLast", &OBConversion::IsLast)
		.function("IsFirstInput", &OBConversion::IsFirstInput)
		.function("SetFirstInput", &OBConversion::SetFirstInput)
		.function("GetOutputIndex", &OBConversion::GetOutputIndex)
		.function("SetOutputIndex", &OBConversion::SetOutputIndex)
		.function("SetMoreFilesToCome", &OBConversion::SetMoreFilesToCome)
		.function("SetOneObjectOnly", &OBConversion::SetOneObjectOnly)
		.function("SetLast", &OBConversion::SetLast)
		.function("IsLastFile", &OBConversion::IsLastFile)
		.function("GetCount", &OBConversion::GetCount)

		.function("WriteString", &OBConversion::WriteString, allow_raw_pointers())
		.function("CloseOutFile", &OBConversion::CloseOutFile)
		.function("ReadString", &OBConversion::ReadString, allow_raw_pointers())
		.function("ReportNumberConverted", &OBConversion::ReportNumberConverted, allow_raw_pointers())
		.function("NumInputObjects", &OBConversion::NumInputObjects)
        ;

	enum_<OBConversion::Option_type>("ObConversion_Option_type")
        .value("INOPTIONS", OBConversion::Option_type::INOPTIONS)
        .value("OUTOPTIONS", OBConversion::Option_type::OUTOPTIONS)
		.value("GENOPTIONS", OBConversion::Option_type::GENOPTIONS)
		.value("ALL", OBConversion::Option_type::ALL)
        ;	
}

EMSCRIPTEN_BINDINGS(OBConvWrapper_Bind) {
    class_<ObConversionWrapper>("ObConversionWrapper")
        .constructor<>()
        .function("getObConversion", &ObConversionWrapper::getObConversion, allow_raw_pointers())
		.function("getInFormat", &ObConversionWrapper::getInFormat, allow_raw_pointers())
		.function("getOutFormat", &ObConversionWrapper::getOutFormat, allow_raw_pointers())
		.function("setInFormat", &ObConversionWrapper::setInFormat, allow_raw_pointers())
		.function("setOutFormat", &ObConversionWrapper::setOutFormat, allow_raw_pointers())
		.function("readString", &ObConversionWrapper::readString, allow_raw_pointers())
		.function("writeString", &ObConversionWrapper::writeString, allow_raw_pointers())
		.function("setInStr", &ObConversionWrapper::setInStr)
		.function("getOutStr", &ObConversionWrapper::getOutStr)
		.function("clearOut", &ObConversionWrapper::clearOut)
		/*
		.function("convert", &ObConversionWrapper::convert)
		.function("getChemObject", &ObConversionWrapper::getChemObject, allow_raw_pointers())
		.function("isLastChemObject", &ObConversionWrapper::isLastChemObject)
		*/
		.function("readFromInput", &ObConversionWrapper::readFromInput, allow_raw_pointers())
		.function("writeToOutput", &ObConversionWrapper::writeToOutput, allow_raw_pointers())
		.function("getSupportedInputFormatsStr", &ObConversionWrapper::getSupportedInputFormatsStr)
		.function("getSupportedOutputFormatsStr", &ObConversionWrapper::getSupportedOutputFormatsStr)
		.function("getFormatInfoById", &ObConversionWrapper::getFormatInfoById)
		.function("addOption", &ObConversionWrapper::addOption)
		.function("removeOption", &ObConversionWrapper::removeOption)
		.function("setOptions", &ObConversionWrapper::setOptions)
		;	
	/*	
	enum_<OBConversion::Option_type>("ObConversion_Option_type")
        .value("INOPTIONS", OBConversion::Option_type::INOPTIONS)
        .value("OUTOPTIONS", OBConversion::Option_type::OUTOPTIONS)
		.value("GENOPTIONS", OBConversion::Option_type::GENOPTIONS)
		.value("ALL", OBConversion::Option_type::ALL)
        ;
	*/
}

EMSCRIPTEN_BINDINGS(OB3DGenWrapper_Bind) {
    class_<OB3DGenWrapper>("OB3DGenWrapper")
        .constructor<>()
		.function("generate3DStructure", &OB3DGenWrapper::generate3DStructure, allow_raw_pointers())
		.function("generate3DStructureFromMolData", &OB3DGenWrapper::generate3DStructureFromMolData, allow_raw_pointers())
		;
}

EMSCRIPTEN_BINDINGS(ObUtils_Bind) {
    class_<ObUtils>("ObUtils")
        .class_function("getAtomType", &ObUtils::getAtomType, allow_raw_pointers())
        ;
}

EMSCRIPTEN_BINDINGS(OBForceField_Bind) {	
    class_<OBForceField, base<OBPlugin>>("OBForceField")        
    	//.constructor<>()
		.class_function("FindForceField", select_overload<OBForceField*(const std::string&)>(&OBForceField::FindForceField), allow_raw_pointers())
		.function("GetUnit", &OBForceField::GetUnit)
		.function("HasAnalyticalGradients", &OBForceField::HasAnalyticalGradients)
		.function("Setup", select_overload<bool(OBMol&)>(&OBForceField::Setup))
		.function("IsSetupNeeded", &OBForceField::IsSetupNeeded)
		.function("GetAtomTypes", &OBForceField::GetAtomTypes)
		.function("GetPartialCharges", &OBForceField::GetPartialCharges)
		.function("GetCoordinates", &OBForceField::GetCoordinates)
		.function("GetConformers", &OBForceField::GetConformers)
		.function("SetCoordinates", &OBForceField::SetCoordinates)
		.function("SetConformers", &OBForceField::SetConformers)
		.function("EnableCutOff", &OBForceField::EnableCutOff)
		.function("IsCutOffEnabled", &OBForceField::IsCutOffEnabled)
		.function("SetVDWCutOff", &OBForceField::SetVDWCutOff)
		.function("GetVDWCutOff", &OBForceField::GetVDWCutOff)
		.function("SetElectrostaticCutOff", &OBForceField::SetElectrostaticCutOff)
		.function("GetElectrostaticCutOff", &OBForceField::GetElectrostaticCutOff)
		.function("SetUpdateFrequency", &OBForceField::SetUpdateFrequency)
		.function("GetUpdateFrequency", &OBForceField::GetUpdateFrequency)
		.function("UpdatePairsSimple", &OBForceField::UpdatePairsSimple)
		.function("GetNumPairs", &OBForceField::GetNumPairs)
		.function("GetNumElectrostaticPairs", &OBForceField::GetNumElectrostaticPairs)
		.function("GetNumVDWPairs", &OBForceField::GetNumVDWPairs)
		.function("EnableAllPairs", &OBForceField::EnableAllPairs)
		.function("Energy", &OBForceField::Energy)
		.function("E_Bond", &OBForceField::E_Bond)
		.function("E_Angle", &OBForceField::E_Angle)
		.function("E_StrBnd", &OBForceField::E_StrBnd)
		.function("E_Torsion", &OBForceField::E_Torsion)
		.function("E_OOP", &OBForceField::E_OOP)
		.function("E_VDW", &OBForceField::E_VDW)
		.function("E_Electrostatic", &OBForceField::E_Electrostatic)
		.function("PrintTypes", &OBForceField::PrintTypes)
		.function("PrintFormalCharges", &OBForceField::PrintFormalCharges)
		.function("PrintPartialCharges", &OBForceField::PrintPartialCharges)
		.function("PrintVelocities", &OBForceField::PrintVelocities)
		.function("SetLogLevel", &OBForceField::SetLogLevel)
		.function("GetLogLevel", &OBForceField::GetLogLevel)
		.function("OBFFLog", select_overload<void(std::string)>(&OBForceField::OBFFLog))
		.function("DistanceGeometry", &OBForceField::DistanceGeometry)
		.function("SystematicRotorSearch", &OBForceField::SystematicRotorSearch)
		.function("SystematicRotorSearchInitialize", &OBForceField::SystematicRotorSearchInitialize)
		.function("SystematicRotorSearchNextConformer", &OBForceField::SystematicRotorSearchNextConformer)
		.function("RandomRotorSearch", &OBForceField::RandomRotorSearch)
		.function("RandomRotorSearchInitialize", &OBForceField::RandomRotorSearchInitialize)
		.function("RandomRotorSearchNextConformer", &OBForceField::RandomRotorSearchNextConformer)
		.function("WeightedRotorSearch", &OBForceField::WeightedRotorSearch)
		.function("FastRotorSearch", &OBForceField::FastRotorSearch)
		.function("SetLineSearchType", &OBForceField::SetLineSearchType)
		.function("GetLineSearchType", &OBForceField::GetLineSearchType)
		.function("LineSearch", select_overload<double(double*, double*)>(&OBForceField::LineSearch), allow_raw_pointers())
		.function("Newton2NumLineSearch", &OBForceField::Newton2NumLineSearch, allow_raw_pointers())
		.function("LineSearchTakeStep", &OBForceField::LineSearchTakeStep, allow_raw_pointers())
		.function("SteepestDescent", &OBForceField::SteepestDescent)
		.function("SteepestDescentInitialize", &OBForceField::SteepestDescentInitialize)
		.function("SteepestDescentTakeNSteps", &OBForceField::SteepestDescentTakeNSteps)
		.function("ConjugateGradients", &OBForceField::ConjugateGradients)
		.function("ConjugateGradientsInitialize", &OBForceField::ConjugateGradientsInitialize)
		.function("ConjugateGradientsTakeNSteps", &OBForceField::ConjugateGradientsTakeNSteps)
		.function("GenerateVelocities", &OBForceField::GenerateVelocities)
		.function("CorrectVelocities", &OBForceField::CorrectVelocities)
		.function("MolecularDynamicsTakeNSteps", &OBForceField::MolecularDynamicsTakeNSteps)
		.function("SetFixAtom", &OBForceField::SetFixAtom)
		.function("UnsetFixAtom", &OBForceField::UnsetFixAtom)
		.function("SetIgnoreAtom", &OBForceField::SetIgnoreAtom)
		.function("UnsetIgnoreAtom", &OBForceField::UnsetIgnoreAtom)
        ;
}

EMSCRIPTEN_BINDINGS(OBPlugin_Bind) {
    class_<OBPlugin>("OBPlugin")
    	//.constructor<>()
		//.function("GetPlugin", &OBPlugin::GetPlugin, allow_raw_pointers())
		.class_function("GetPlugin", optional_override([](std::string Type, std::string ID){
				return OBPlugin::GetPlugin(Type.c_str(), ID.c_str());
			}), allow_raw_pointers())
		//.function("Description", &OBPlugin::Description, allow_raw_pointers())
		.function("Description", optional_override([](OBPlugin& self){
				std::string s = self.Description();
				return s;
			}))
		//.function("TypeID", &OBPlugin::TypeID, allow_raw_pointers())
		.function("TypeID", optional_override([](OBPlugin& self){
				std::string s = self.TypeID();
				return s;
			}))
		//.function("Display", &OBPlugin::Display, allow_raw_pointers())
		.function("Init", &OBPlugin::Init)
		//.function("GetID", &OBPlugin::GetID, allow_raw_pointers())
		.function("GetID", optional_override([](OBPlugin& self){
				std::string s = self.GetID();
				return s;
			}))
		;
}

EMSCRIPTEN_BINDINGS(OBOp_Bind) {
    class_<OBOp, base<OBPlugin>>("OBOp")	
    	//.constructor<>()		
		.class_function("FindType", optional_override([](const std::string id){
				return OBOp::FindType(id.c_str());
			}), allow_raw_pointers())
		//.function("Do", &OBOp::Do, allow_raw_pointers())
		.function("Do", optional_override([](OBOp& self, OBBase *pOb, const std::string OptionText = ""){
				return self.Do(pOb, OptionText.c_str());
			}), allow_raw_pointers())
		.function("WorksWith", &OBOp::WorksWith, allow_raw_pointers())
		.function("OpOptions", &OBOp::OpOptions, allow_raw_pointers())
		//.function("DoOps", &OBOp::DoOps, allow_raw_pointers())
		;
}
/*
EMSCRIPTEN_BINDINGS(OBOpHelper_Bind) {
    class_<OBOpHelper>("OBOpHelper")	
		.class_function("FindType", &OBOpHelper::FindType, allow_raw_pointers())
		;
}
*/




/*
EMSCRIPTEN_BINDINGS(OBFormat_Bind) {
    class_<OBMol>("OBFormat")
        .constructor<void>()
        ;
}
EMSCRIPTEN_BINDINGS(OBConversion_Bind) {
    class_<OBConversion>("OBConversion")
        //.constructor<void>()
        //.function("ReadString", &OBConversion::ReadString)
		//.function("WriteString", &OBConversion::WriteString)
		//.function("SetInFormat",  select_overload<void(OBFormat)>(&OBConversion::SetInFormat))
		//.function("SetOutFormat",  select_overload<void(OBFormat)>&OBConversion::SetOutFormat)
		//.function("ReportNumberConverted", &OBConversion::ReportNumberConverted)
        //;
}

*/


extern "C"{

/*
const char* obGetSupportedFormatsDetailStr(char* inOrOut, char* formatDelimiter, char* fieldDelimiter)
{
	OBConversion obConv;
	std::vector<std::string> fmts;
	if (strcmp(inOrOut, "in") == 0)
		fmts = obConv.GetSupportedInputFormat();
	else
		fmts = obConv.GetSupportedOutputFormat();

	//obErrorLog.ThrowError(__FUNCTION__, "total: " + std::to_string(fmts.size()) + formatDelimiter + fieldDelimiter, obWarning);

	std::string formatNameSep = " --";

	std::string result = "", s = "", sid = "", sDes, sMimeType, sitem = "";
	int i = 0, pos;
	for (vector<std::string>::iterator iter=fmts.begin();iter!=fmts.end();iter++)
	{
		s = *iter;
		//obErrorLog.ThrowError(__FUNCTION__, s, obWarning);
		// id
		pos = s.find(formatNameSep);
		if (pos >= 0)
			sid = s.substr(0, pos);
		else
			sid = s;
		

		OBFormat* fmt = obConv.FindFormat(sid);
		if (fmt)
		{
			sDes = fmt->FirstLine(fmt->Description());
			sMimeType = fmt->GetMIMEType();
			if (sMimeType.length() <= 2)  // less then 3 chars is not a legal mime type
				sMimeType = "";
			else
			{
				// correct some MIME type mistakes in original OpenBabel code
				if ((sid == "sd") || (sid == "sdf"))
					sMimeType = "chemical/x-mdl-sdfile";				
			}

			// add infos
			sitem = "";
			sitem.append(sid);
			sitem.append(fieldDelimiter);
			sitem.append(sMimeType);
			sitem.append(fieldDelimiter);
			sitem.append(sDes);
		}

		result.append(sitem);
		if (i != 0)
			result.append(formatDelimiter);
		++i;
	}

	obErrorLog.ThrowError(__FUNCTION__, "result: " + result, obWarning);

	return result.c_str();	
}
*/
/*
const char* obGetSupportedOutputFormatsDetailStr(char* formatDelimiter)
{
	ObConversionWrapper w;
	std::string s = w.getSupportedOutputFormatsStr(formatDelimiter);
	return s.c_str();
}
*/

}