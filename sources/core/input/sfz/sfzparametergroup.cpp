/***************************************************************************
**                                                                        **
**  Polyphone, a soundfont editor                                         **
**  Copyright (C) 2013-2024 Davy Triponney                                **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program. If not, see http://www.gnu.org/licenses/.    **
**                                                                        **
****************************************************************************
**           Author: Davy Triponney                                       **
**  Website/Contact: https://www.polyphone.io                             **
**             Date: 01.01.2013                                           **
***************************************************************************/

#include "sfzparametergroup.h"
#include "soundfontmanager.h"

void SfzParameterGroup::moveOpcodesInGlobal(SfzParameterRegion &globalZone)
{
    _paramGlobaux.mergeIfNotDefined(globalZone);
}

void SfzParameterGroup::moveKeynumInSamples(SfzParameter::OpCode opCodeKeynum, SfzParameter::OpCode opCodeBase)
{
    if (_paramGlobaux.isDefined(opCodeKeynum))
    {
        double valueKeynum = _paramGlobaux.getDoubleValue(opCodeKeynum);
        double valueBase = 0.001;
        if (_paramGlobaux.isDefined(opCodeBase))
            valueBase = _paramGlobaux.getDoubleValue(opCodeBase);
        for (int i = 0; i < _regionList.size(); i++)
        {
            if (!_regionList.at(i).isDefined(opCodeKeynum))
                _regionList[i] << SfzParameter(opCodeKeynum, valueKeynum);
            if (!_regionList.at(i).isDefined(opCodeBase))
                _regionList[i] << SfzParameter(opCodeBase, valueBase);
        }

        _paramGlobaux.removeOpCode(opCodeKeynum);
    }
}

void SfzParameterGroup::moveOpcodeInSamples(SfzParameter::OpCode opcode, QMetaType::Type type)
{
    switch (type)
    {
    case QMetaType::Double:
        if (_paramGlobaux.isDefined(opcode))
        {
            double value = _paramGlobaux.getDoubleValue(opcode);
            for (int i = 0; i < _regionList.size(); i++)
                if (!_regionList.at(i).isDefined(opcode))
                    _regionList[i] << SfzParameter(opcode, value);
        }
        break;
    case QMetaType::Int:
        if (_paramGlobaux.isDefined(opcode))
        {
            int value = _paramGlobaux.getIntValue(opcode);
            for (int i = 0; i < _regionList.size(); i++)
                if (!_regionList.at(i).isDefined(opcode))
                    _regionList[i] << SfzParameter(opcode, value);
        }
        break;
    case QMetaType::QString:
        if (_paramGlobaux.isDefined(opcode))
        {
            QString value = _paramGlobaux.getStrValue(opcode);
            for (int i = 0; i < _regionList.size(); i++)
                if (!_regionList.at(i).isDefined(opcode))
                    _regionList[i] << SfzParameter(opcode, value);
        }
        break;
    default:
        break;
    }
    _paramGlobaux.removeOpCode(opcode);
}

void SfzParameterGroup::moveModInSamples()
{
    QList<SfzParameter::OpCode> listTmp;
    listTmp << SfzParameter::op_modEnvToTon
            << SfzParameter::op_pitcheg_attack
            << SfzParameter::op_pitcheg_decay
            << SfzParameter::op_pitcheg_delay
            << SfzParameter::op_pitcheg_hold
            << SfzParameter::op_pitcheg_release
            << SfzParameter::op_pitcheg_sustain;
    moveModInSamples(listTmp);

    listTmp.clear();
    listTmp << SfzParameter::op_modEnvToFilter
            << SfzParameter::op_fileg_attack
            << SfzParameter::op_fileg_decay
            << SfzParameter::op_fileg_delay
            << SfzParameter::op_fileg_hold
            << SfzParameter::op_fileg_release
            << SfzParameter::op_fileg_sustain;
    moveModInSamples(listTmp);

    listTmp.clear();
    listTmp << SfzParameter::op_modLFOtoVolume
            << SfzParameter::op_modLFOdelay
            << SfzParameter::op_modLFOfreq;
    moveModInSamples(listTmp);

    listTmp.clear();
    listTmp << SfzParameter::op_modLFOtoFilter
            << SfzParameter::op_filLFOdelay
            << SfzParameter::op_filLFOfreq;
    moveModInSamples(listTmp);
}

void SfzParameterGroup::moveModInSamples(QList<SfzParameter::OpCode> opCodeList)
{
    for (int i = 0; i < _regionList.size(); i++)
    {
        for (int j = 0; j < opCodeList.size(); j++)
        {
            SfzParameter::OpCode opCode = opCodeList.at(j);
            if (!_regionList.at(i).isDefined(opCode) && _paramGlobaux.isDefined(opCode))
            {
                // On regarde si d'autres opcodes associés sont présents
                bool isPresent = false;
                for (int k = 0; k < opCodeList.size(); k++)
                    if (k != j)
                        isPresent |= _regionList.at(i).isDefined(opCodeList.at(k));
                if (isPresent)
                {
                    if (opCode == SfzParameter::op_modEnvToTon || opCode == SfzParameter::op_modEnvToFilter ||
                            opCode == SfzParameter::op_modLFOtoVolume || opCode == SfzParameter::op_modLFOtoFilter)
                        _regionList[i] << SfzParameter(opCode, _paramGlobaux.getIntValue(opCode));
                    else
                        _regionList[i] << SfzParameter(opCode, _paramGlobaux.getDoubleValue(opCode));
                }
            }
        }
    }
}

void SfzParameterGroup::adjustVolume(double offset)
{
    _paramGlobaux.adjustVolume(offset);
    for (int i = 0; i < _regionList.size(); i++)
        _regionList[i].adjustVolume(offset);
}

void SfzParameterGroup::adjustCorrection(QString path)
{
    int defaultCorrection = _paramGlobaux.getIntValue(SfzParameter::op_tuningFine);
    for (int i = 0; i < _regionList.size(); i++)
        _regionList[i].adjustCorrection(path, defaultCorrection);
}

void SfzParameterGroup::adjustModulationVolume()
{
    double correctionGlobale = 0;
    if (_paramGlobaux.isDefined(SfzParameter::op_modLFOtoVolume))
    {
        correctionGlobale = qAbs(_paramGlobaux.getDoubleValue(SfzParameter::op_modLFOtoVolume));
        _paramGlobaux.adjustVolume(-correctionGlobale);
    }
    for (int i = 0; i < _regionList.size(); i++)
    {
        double correction;
        if (_regionList.at(i).isDefined(SfzParameter::op_modLFOtoVolume))
            correction = qAbs(_regionList.at(i).getDoubleValue(SfzParameter::op_modLFOtoVolume));
        else
            correction = correctionGlobale;
        if (correction != 0)
            _regionList[i].adjustVolume(-correction);
    }
}

void SfzParameterGroup::checkSampleValid(QString path)
{
    int size = _regionList.size();
    for (int i = size - 1; i >= 0; i--)
        if (!_regionList[i].sampleValid(path))
            _regionList.removeAt(i);
}

void SfzParameterGroup::checkFilter()
{
    _paramGlobaux.checkFilter();
    for (int i = 0; i < _regionList.size(); i++)
        _regionList[i].checkFilter();
}

void SfzParameterGroup::checkKeyTrackedFilter()
{
    _paramGlobaux.checkKeyTrackedFilter(false);
    for (int i = 0; i < _regionList.size(); i++)
        _regionList[i].checkKeyTrackedFilter(true);
}

bool SfzParameterGroup::isChannel10()
{
    return (_paramGlobaux.getIntValue(SfzParameter::op_chanMin) == 10 &&
            _paramGlobaux.getIntValue(SfzParameter::op_chanMax) == 10);
}

double SfzParameterGroup::getAmpliMax()
{
    double ampliMax = _paramGlobaux.getDoubleValue(SfzParameter::op_volume);
    for (int i = 0; i < _regionList.size(); i++)
        ampliMax = qMax(ampliMax, _regionList.at(i).getDoubleValue(SfzParameter::op_volume));
    return ampliMax;
}

void SfzParameterGroup::decode(SoundfontManager * sf2, EltID idInst, QString pathSfz)
{
    // Fill the parameters of the global division
    _paramGlobaux.decode(sf2, idInst);

    // Deactivate default modulators
    disableModulators(sf2, idInst);

    // Link to samples
    EltID idInstSmpl = idInst;
    idInstSmpl.typeElement = elementInstSmpl;
    EltID idSmpl = idInst;
    idSmpl.typeElement = elementSmpl;
    AttributeValue val;
    for (int i = 0; i < _regionList.size(); i++)
    {
        // Create samples if needed and get their index
        QList<int> listeIndexSmpl = _regionList.at(i).getSampleIndex(sf2, idInst, pathSfz);

        // Process possible offsets
        if (!listeIndexSmpl.isEmpty())
        {
            idSmpl.indexElt = listeIndexSmpl.first();
            _regionList[i].adaptOffsets(sf2->get(idSmpl, champ_dwStartLoop).dwValue,
                                            sf2->get(idSmpl, champ_dwEndLoop).dwValue,
                                            sf2->get(idSmpl, champ_dwLength).dwValue);
        }

        if (_regionList.at(i).isDefined(SfzParameter::op_pan) && listeIndexSmpl.size() == 2)
        {
            double pan = _regionList.at(i).getDoubleValue(SfzParameter::op_pan);
            if (qAbs(pan) >= 99.9)
            {
                // Passage en mono
                if (pan < 0)
                    listeIndexSmpl.removeAt(1);
                else
                    listeIndexSmpl.removeAt(0);
            }
        }

        if (listeIndexSmpl.size() == 1) // Mono sample
        {
            // Create a division InstSmpl
            idInstSmpl.indexElt2 = sf2->add(idInstSmpl);

            // Link to the sample
            val.wValue = listeIndexSmpl.first();
            sf2->set(idInstSmpl, champ_sampleID, val);

            // Fill the parameters of this division
            _regionList.at(i).decode(sf2, idInstSmpl);

            if (_regionList.at(i).isDefined(SfzParameter::op_pan))
            {
                val.shValue = 5 * _regionList.at(i).getDoubleValue(SfzParameter::op_pan);
                sf2->set(idInstSmpl, champ_pan, val);
            }
        }
        else if (listeIndexSmpl.size() == 2) // Sample stereo
        {
            // Process width / position
            double width = 500;
            if (_regionList.at(i).isDefined(SfzParameter::op_width))
                width = 5. * _regionList.at(i).getDoubleValue(SfzParameter::op_width);
            double position = 0;
            if (_regionList.at(i).isDefined(SfzParameter::op_position))
                position = _regionList.at(i).getDoubleValue(SfzParameter::op_position) / 100.;
            if (position < 0)
                position = -qAbs(position * (500 - qAbs(width)));
            else
                position = qAbs(position * (500 - qAbs(width)));

            // Process pan
            double attenuation = 0;
            int panDefined = -1;
            if (_regionList.at(i).isDefined(SfzParameter::op_pan))
            {
                double pan = _regionList.at(i).getDoubleValue(SfzParameter::op_pan);
                attenuation = -SfzParameterRegion::percentToDB(100 - qAbs(pan));
                if (pan < 0)
                    panDefined = 1;
                else if (pan > 0)
                    panDefined = 0;
            }

            for (int j = 0; j < listeIndexSmpl.size(); j++)
            {
                // Creation a division InstSmpl
                idInstSmpl.indexElt2 = sf2->add(idInstSmpl);

                // Link to the sample
                val.wValue = listeIndexSmpl.at(j);
                sf2->set(idInstSmpl, champ_sampleID, val);

                // Pan
                if (panDefined == j)
                {
                    val.shValue = qRound(10. * attenuation / DB_SF2_TO_REAL_DB);
                    sf2->set(idInstSmpl, champ_initialAttenuation, val);
                }

                // Fill the parameters of this division
                _regionList.at(i).decode(sf2, idInstSmpl);

                // Width, position
                if (j == 0)
                    val.shValue = -width + position;
                else
                    val.shValue = width + position;
                sf2->set(idInstSmpl, champ_pan, val);
            }
        }
    }
}

void SfzParameterGroup::disableModulators(SoundfontManager * sm, EltID idInst)
{
    EltID idMod(elementInstMod, idInst.indexSf2, idInst.indexElt);
    AttributeValue val;

    // Disable "MIDI Note-On Velocity to Initial Attenuation"
    idMod.indexMod = sm->add(idMod);
    val.sfModValue = SFModulator(GeneralController::GC_noteOnVelocity, ModType::typeConcave, true, false);
    sm->set(idMod, champ_sfModSrcOper, val);
    val.wValue = champ_initialAttenuation;
    sm->set(idMod, champ_sfModDestOper, val);
    val.wValue = 0;
    sm->set(idMod, champ_modAmount, val);
    val.sfModValue = SFModulator(GeneralController::GC_noController, ModType::typeLinear, false, false);
    sm->set(idMod, champ_sfModAmtSrcOper, val);
    val.sfTransValue = SFTransform::linear;
    sm->set(idMod, champ_sfModTransOper, val);

    // Disable "MIDI Note-On Velocity to Filter Cutoff"
    idMod.indexMod = sm->add(idMod);
    val.sfModValue = SFModulator(GeneralController::GC_noteOnVelocity, ModType::typeLinear, true, false);
    sm->set(idMod, champ_sfModSrcOper, val);
    val.wValue = champ_initialFilterFc;
    sm->set(idMod, champ_sfModDestOper, val);
    val.wValue = 0;
    sm->set(idMod, champ_modAmount, val);
    val.sfModValue = SFModulator(GeneralController::GC_noController, ModType::typeLinear, false, false);
    sm->set(idMod, champ_sfModAmtSrcOper, val);
    val.sfTransValue = SFTransform::linear;
    sm->set(idMod, champ_sfModTransOper, val);
}
