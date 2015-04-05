/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2014 Gerard Visser.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include <stddef.h>
#include <lombok/base/errors.h>
#include <lombok/parsing/contextCodes.h>
#include "asmErrors.h"
#include "asmTokenIDs.h"
#include "orderedLabel.h"
#include "programmeBuilder.h"
#include "expressions/xRelValue.h"
#include "instructions/conditionalJump.h"
#include "instructions/fixedSizeInstruction.h"

REFL_IMPL (programmeBuilder_t, object_t)

using namespace pars;

static void validateConditionalJumps (arrayList_t* byteBlocks);

programmeBuilder_t::programmeBuilder_t (void) {
  ___CBTPUSH;

  m_csLabel = new string_t ("CS");
  m_labels = new hashMap_t (1024);
  m_instructionFactory = new instructionFactory_t (m_labels);

  ___CBTPOP;
}

programmeBuilder_t::~programmeBuilder_t (void) {
  ___CBTPUSH;

  m_labels->dump ();
  m_csLabel->dump ();
  m_instructionFactory->dump ();

  ___CBTPOP;
}

int programmeBuilder_t::addByteBlockOrLabel (arrayList_t* byteBlocks,
                                             arrayList_t* orderedLabels,
                                          listIterator_t* tokens,
                                                 token_t* token,
                                                xValue_t* csOffset,
                                                      int locationIndex) {
  ___CBTPUSH;

  int tokenId = token->id ();
  switch (tokenId & MASK_BLTYPE) {
  case BLTYPE_PRIMARY:
    switch (tokenId & MASK_TID_PCAT) {
    case TID_CAT_LABEL: {
      int labelIndex = byteBlocks->length ();
      xValue_t* labelValue = (xValue_t*) m_labels->get (token->text ());
      orderedLabels->append (new orderedLabel_t (labelIndex, labelValue));
      labelValue->m_value = locationIndex;
      switch (tokenId) {
      case TID_P_CS_L:
        /* Note: data is expected before code and the size of the data is assumed to be constant,
                 so this is the offset of the code segment relative to the data segment (it will
                 not change because some instructions might turn out to be smaller than initially
                 expected). */
        csOffset->m_value = locationIndex & 0xFFFFFFFFFFFFFFF0;
        break;

      case TID_P_DS_L:
        if (locationIndex != 0) {
          asmErrors::exit ("label DS should be defined before any code or data", token);
        }
        break;
      }
    } break;

    case TID_CAT_INSTRUCTION: {
      byteBlock_t* byteBlock = m_instructionFactory->createInstruction (tokens, token, csOffset);
      if (m_instructionFactory->segmentOverrideIID () != 0) {
        byteBlock_t* segmentOverrideInstruction = new fixedSizeInstruction_t (csOffset, m_instructionFactory->segmentOverrideIID (), 1);
        segmentOverrideInstruction->m_location->m_value = locationIndex++;
        byteBlocks->append (segmentOverrideInstruction);
      }
      int size = byteBlock->size ();
      byteBlock->m_location->m_value = locationIndex;
      locationIndex += size;
      byteBlocks->append (byteBlock);
    } break;

    default:
      asmErrors::exit ("label or instruction expected", token);
    }
    break;

  default:
    asmErrors::exit ("label or instruction expected", token);
  }

  ___CBTPOP;
  return locationIndex;
}

programme_t* programmeBuilder_t::build (arrayList_t* tokens) {
  ___CBTPUSH;

  xValue_t* csOffset = new xValue_t ();
  findLabels (tokens, csOffset);

  int locationIndex = 0;
  arrayList_t* byteBlocks = new arrayList_t (1024);
  arrayList_t* orderedLabels = new arrayList_t (m_labels->length ());
  listIterator_t* tokenIterator = tokens->iterator ();
  token_t* token = (token_t*) tokenIterator->next ();
  while (token->contextCode () != CTXT_LAST_TOKEN) {
    locationIndex = addByteBlockOrLabel (byteBlocks, orderedLabels, tokenIterator, token, csOffset, locationIndex);
    token = (token_t*) tokenIterator->next ();
  }
  determineLabelValues (byteBlocks, orderedLabels);
  tokenIterator->dump ();
  orderedLabels->dump ();
  csOffset->dump ();

  validateConditionalJumps (byteBlocks);

  int codeOffset = 0;
  xValue_t* csLabelValue = (xValue_t*) m_labels->get (m_csLabel);
  if (csLabelValue != NULL) {
    codeOffset = csLabelValue->get ();
  }
  programme_t* programme = new programme_t (byteBlocks, codeOffset);

  ___CBTPOP;
  return programme;
}

void programmeBuilder_t::determineLabelValues (arrayList_t* byteBlocks, arrayList_t* orderedLabels) {
  ___CBTPUSH;

  bool labelValuesUnstable;
  const int labelCount = orderedLabels->length ();
  const int blockCount = byteBlocks->length ();
  do {
    int bi = 0;
    int li = 0;
    int blockLocation = 0;
    labelValuesUnstable = false;
    while (li < labelCount) {
      orderedLabel_t* label = (orderedLabel_t*) orderedLabels->get (li);
      int nextBlockIndexForLabelValue = label->index ();
      while (bi < nextBlockIndexForLabelValue) {
        byteBlock_t* byteBlock = (byteBlock_t*) byteBlocks->get (bi);
        byteBlock->m_location->m_value = blockLocation;
        blockLocation += byteBlock->size ();
        ++bi;
      }
      xValue_t* labelValue = label->value ();
      labelValuesUnstable = labelValuesUnstable || labelValue->m_value != blockLocation;
      labelValue->m_value = blockLocation;
      ++li;
    }
    while (bi < blockCount) {
      byteBlock_t* byteBlock = (byteBlock_t*) byteBlocks->get (bi);
      byteBlock->m_location->m_value = blockLocation;
      blockLocation += byteBlock->size ();
      ++bi;
    }
  } while (labelValuesUnstable);

  ___CBTPOP;
}

void programmeBuilder_t::findLabels (arrayList_t* tokens, xValue_t* csOffset) {
  ___CBTPUSH;

  m_labels->clear ();
  csOffset->m_value = 0;
  const int len = tokens->length () - 1;

  for (int i = 0; i < len; ++i) {
    token_t* token = (token_t*) tokens->get (i);
    if ((token->id () & MASK_TID_PCAT) == TID_CAT_LABEL) {
      string_t* label = token->text ();
      if (m_labels->contains (label)) {
        asmErrors::exit ("redefinition of label \x91%t\x92", token);
      } else if (token->id () == TID_CAT_LABEL) {
        label->reserve ();
        m_labels->set (label, new xRelValue_t (csOffset));
      } else {
        label->reserve ();
        m_labels->set (label, new xValue_t ());
      }
    }
  }

  ___CBTPOP;
}

static void validateConditionalJumps (arrayList_t* byteBlocks) {
  ___BTPUSH;

  const int blockCount = byteBlocks->length ();
  for (int i = 0; i < blockCount; ++i) {
    byteBlock_t* byteBlock = (byteBlock_t*) byteBlocks->get (i);
    if (byteBlock->type () == byteBlock_t::TYPE_INSTRUCTION && ((instruction_t*) byteBlock)->group () == GRP_CONJMP) {
      conditionalJump_t* jumpInstruction = (conditionalJump_t*) byteBlock;
      int jumpVector = jumpInstruction->operand1 ()->value () - jumpInstruction->location () - 2;
      if (jumpVector < -0x80 || jumpVector > 0x7F) {
        asmErrors::exit ("Address to jump to out of reach", jumpInstruction->line (), jumpInstruction->column ());
      }
    }
  }

  ___BTPOP;
}
