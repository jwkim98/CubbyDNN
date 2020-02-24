// Copyright (c) 2019 Chris Ohk, Justin Kim

// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <cubbydnn/Computations/Functions/Matrix.hpp>
#include <cubbydnn/Units/HiddenComputableUnits/HiddenUnit.hpp>

namespace CubbyDNN
{
HiddenUnit::HiddenUnit(std::vector<TensorInfo> inputTensorInfoVector,
                       std::vector<TensorInfo> outputTensorInfoVector)
    : ComputableUnit(std::move(inputTensorInfoVector),
                     std::move(outputTensorInfoVector), UnitType::Hidden)
{
    m_outputPtrVector =
        std::vector<SharedPtr<ComputableUnit>>(m_outputTensorInfoVector.size());
    m_inputPtrVector =
        std::vector<SharedPtr<ComputableUnit>>(m_inputTensorInfoVector.size());

    m_inputTensorVector.reserve(m_inputTensorInfoVector.size());
    for (auto& inputTensorInfo : m_inputTensorInfoVector)
    {
        m_inputTensorVector.emplace_back(AllocateTensor(inputTensorInfo));
    }

    m_outputTensorVector.reserve(m_outputTensorInfoVector.size());
    for (auto& outputTensorInfo : m_outputTensorInfoVector)
    {
        m_outputTensorVector.emplace_back(AllocateTensor(outputTensorInfo));
    }
}

bool HiddenUnit::IsReady()
{
    if (m_unitState.IsBusy)
        return false;

    for (auto& elem : m_inputPtrVector)
    {
        if (elem->GetStateNum() != this->GetStateNum() + 1)
            return false;
    }

    for (auto& elem : m_outputPtrVector)
    {
        if (elem->GetStateNum() != this->GetStateNum())
            return false;
    }

    return true;
}

MatMul::MatMul(const TensorInfo& inputA, const TensorInfo& inputB,
               const TensorInfo& output)
    : HiddenUnit({ inputA, inputB }, { output })
{
    assert(inputA.GetShape().Col == inputB.GetShape().Row);
    assert(inputA.GetShape().Batch == inputB.GetShape().Batch &&
        inputA.GetShape().Batch == output.GetShape().Batch);;
    assert(inputA.GetShape().Channel == inputB.GetShape().Channel &&
        inputA.GetShape().Channel == output.GetShape().Channel);
}

void MatMul::Compute()
{
    MultiplyOp(m_inputTensorVector.at(0), m_inputTensorVector.at(1),
             m_outputTensorVector.at(0));
}
} // namespace CubbyDNN