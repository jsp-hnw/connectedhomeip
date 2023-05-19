package com.google.chip.chiptool.clusterclient

import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import androidx.lifecycle.lifecycleScope
import chip.devicecontroller.ChipDeviceController
import com.google.chip.chiptool.ChipClient
import com.google.chip.chiptool.GenericChipDeviceListener
import com.google.chip.chiptool.R
import com.google.chip.chiptool.databinding.OpCredClientFragmentBinding
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.launch

import chip.devicecontroller.ClusterIDMapping.*
import chip.devicecontroller.ReportCallback
import chip.devicecontroller.model.ChipAttributePath
import chip.devicecontroller.model.ChipEventPath
import chip.devicecontroller.model.NodeState

class OpCredClientFragment : Fragment() {
  private val deviceController: ChipDeviceController
    get() = ChipClient.getDeviceController(requireContext())

  private lateinit var scope: CoroutineScope

  private lateinit var addressUpdateFragment: AddressUpdateFragment

  private var _binding: OpCredClientFragmentBinding? = null
  private val binding get() = _binding!!

  override fun onCreateView(
    inflater: LayoutInflater,
    container: ViewGroup?,
    savedInstanceState: Bundle?
  ): View {
    _binding = OpCredClientFragmentBinding.inflate(inflater, container, false)
    scope = viewLifecycleOwner.lifecycleScope

    deviceController.setCompletionListener(ChipControllerCallback())

    addressUpdateFragment =
      childFragmentManager.findFragmentById(R.id.addressUpdateFragment) as AddressUpdateFragment

    binding.readSupportedFabricBtn.setOnClickListener { scope.launch { readClusterAttribute(OperationalCredentials.Attribute.SupportedFabrics) } }
    binding.readCommissionedFabricBtn.setOnClickListener { scope.launch { readClusterAttribute(OperationalCredentials.Attribute.CommissionedFabrics) } }

    return binding.root
  }

  override fun onDestroyView() {
    super.onDestroyView()
    _binding = null
  }

  inner class ChipControllerCallback : GenericChipDeviceListener() {
    override fun onConnectDeviceComplete() {}

    override fun onCommissioningComplete(nodeId: Long, errorCode: Int) {
      Log.d(TAG, "onCommissioningComplete for nodeId $nodeId: $errorCode")
    }

    override fun onNotifyChipConnectionClosed() {
      Log.d(TAG, "onNotifyChipConnectionClosed")
    }

    override fun onCloseBleComplete() {
      Log.d(TAG, "onCloseBleComplete")
    }

    override fun onError(error: Throwable?) {
      Log.d(TAG, "onError: $error")
    }
  }

  private suspend fun readClusterAttribute(attribute: OperationalCredentials.Attribute) {
    val endpointId = addressUpdateFragment.endpointId
    val clusterId = OperationalCredentials.ID
    val attributeName = attribute.name
    val attributeId = attribute.id

    val devicePtr = ChipClient.getConnectedDevicePointer(requireContext(), addressUpdateFragment.deviceId)

    ChipClient.getDeviceController(requireContext()).readPath(object: ReportCallback {
      override fun onError(attributePath: ChipAttributePath?, eventPath: ChipEventPath?, ex: java.lang.Exception) {
        showMessage("Read $attributeName failure $ex")
        Log.e(TAG, "Read $attributeName failure", ex)
      }

      override fun onReport(nodeState: NodeState?) {
        val value = nodeState?.getEndpointState(endpointId)?.getClusterState(clusterId)?.getAttributeState(attributeId)?.value ?: "null"
        Log.i(TAG,"OpCred $attributeName value: $value")
        showMessage("OpCred $attributeName value: $value")
      }

    }, devicePtr, listOf(ChipAttributePath.newInstance(endpointId.toLong(), clusterId, attributeId)), null, false, 0 /* imTimeoutMs */)
  }

  private fun showMessage(msg: String) {
    requireActivity().runOnUiThread {
      binding.opCredClusterCommandStatus.text = msg
    }
  }

  override fun onResume() {
    super.onResume()
    addressUpdateFragment.endpointId = OPERATIONAL_CREDENTIALS_ENDPOINT_ID
  }

  companion object {
    private const val TAG = "OpCredClientFragment"
    private const val OPERATIONAL_CREDENTIALS_ENDPOINT_ID = 0
    fun newInstance(): OpCredClientFragment = OpCredClientFragment()
  }
}
