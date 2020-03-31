// Copyright (c) 2019 Chris Ohk, Justin Kim

// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYDNN_COMPUTABLEUNIT_HPP
#define CUBBYDNN_COMPUTABLEUNIT_HPP

#include <atomic>
#include <cubbydnn/Tensors/Tensor.hpp>
#include <cubbydnn/Tensors/TensorInfo.hpp>

namespace CubbyDNN
{
class ComputableUnit
{
public:
    //! \param unitId : type of the unit
    //! \param inputShapeVector : vector of input tensor information
    //! \param outputShape : output tensor information
    //! \param numberSystem : number system to use
    ComputableUnit(UnitId unitId,
                   std::vector<Shape> inputShapeVector,
                   Shape outputShape, NumberSystem numberSystem);

    //! Constructor
    virtual ~ComputableUnit() = default;

    ComputableUnit(const ComputableUnit& computableUnit) = delete;
    ComputableUnit(ComputableUnit&& computableUnit) noexcept;

    ComputableUnit& operator=(ComputableUnit&& computableUnit) noexcept;
    ComputableUnit& operator=(const ComputableUnit& computableUnit) = delete;

    UnitId GetId() const
    {
        return m_id;
    }

    Tensor& GetInputForwardTensor(std::size_t index)
    {
        return m_inputForwardTensorVector.at(index);
    }

    Tensor& GetOutputForwardTensor()
    {
        return m_outputForwardTensor;
    }

    Tensor& GetInputBackwardTensor()
    {
        return m_inputBackwardTensor;
    }

    Tensor& GetOutputBackwardTensor(std::size_t index)
    {
        return m_outputBackwardTensorVector.at(index);
    }

    Shape GetOutputTensorShape() const
    {
        return m_outputTensorShape;
    }

    const std::vector<UnitId>& GetInputUnitVector() const
    {
        return m_inputUnitIdVector;
    }

    const std::vector<std::pair<UnitId, std::size_t>>&
    GetOutputUnitVector() const
    {
        return m_outputUnitIdVector;
    }

    void SetInputUnitVector(std::vector<UnitId> inputUnitVector)
    {
        m_inputUnitIdVector = inputUnitVector;
    }

    void AddOutputUnitVector(UnitId outputUnit, std::size_t inputIndex)
    {
        m_outputUnitIdVector.emplace_back(
            std::make_pair(outputUnit, inputIndex));
    }

    //! Called after computation for releasing the unit after computation
    //! Increments the stateNum and marks IsBusy as false
    void ReleaseUnit();

    //! Gets reference of the atomic state counter for atomic comparison
    //! of state counter
    //! \return : reference of the state counter
    std::size_t GetStateNum() const
    {
        return m_unitState.StateNum.load(std::memory_order_acquire);
    }

    virtual void Forward() = 0;

    virtual void Backward() = 0;

    UnitType Type()
    {
        return m_id.Type;
    }

protected:
    /// UnitState m_objectPtr indicates execution state of ComputableUnit
    UnitState m_unitState;
    //! Unique Id of this unit
    UnitId m_id;
    //! vector of tensor information for input in forward propagation
    std::vector<Shape> m_inputShapeVector;
    //! tensor information for output in forward propagation
    Shape m_outputTensorShape;
    //! Number system for this unit to use
    NumberSystem m_numberSystem;

    std::vector<UnitId> m_inputUnitIdVector;

    std::vector<std::pair<UnitId, std::size_t>> m_outputUnitIdVector;
    //! single output tensor of forward propagation
    Tensor m_outputForwardTensor;
    //! single output tensor of back propagation
    Tensor m_inputBackwardTensor;
    //! vector of input tensors used to compute forward propagation
    std::vector<Tensor> m_inputForwardTensorVector;
    //! vector of output tensors used to compute back propagation
    std::vector<Tensor> m_outputBackwardTensorVector;
};
}; // namespace CubbyDNN

#endif  // CUBBYDNN_COMPUTABLEUNIT_HPP
