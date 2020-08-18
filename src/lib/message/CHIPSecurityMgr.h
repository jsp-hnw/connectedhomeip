/*
 *
 *    Copyright (c) 2020 Project CHIP Authors
 *    Copyright (c) 2013-2017 Nest Labs, Inc.
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

/**
 *    @file
 *      This file defines types and objects for managing CHIP session
 *      security state.
 *
 */

#ifndef CHIPSECURITYMANAGER_H_
#define CHIPSECURITYMANAGER_H_

#include <Profiles/common/CHIPMessage.h>
#include <Profiles/security/CHIPCASE.h>
#include <Profiles/security/CHIPKeyExport.h>
#include <Profiles/security/CHIPPASE.h>
#include <Profiles/security/CHIPTAKE.h>
#include <Profiles/status-report/StatusReportProfile.h>
#include <core/CHIPError.h>
#include <message/CHIPExchangeMgr.h>
#include <support/DLLUtil.h>

/**
 *   @namespace chip::Platform::Security
 *
 *   @brief
 *     Functions in this namespace are to be implemented by platforms that use CHIP,
 *     according to the needs/constraints of the particular environment.
 *
 */

namespace chip {

namespace Platform {
namespace Security {

#if CHIP_CONFIG_SECURITY_MGR_TIME_ALERTS_PLATFORM

/**
 * This function is called to notify the application when a time-consuming
 * cryptographic operation is about to start.
 *
 * @note If application wants to receive these alerts and adjust platform settings
 *       accordingly then it should provide its own implementation of these functions
 *       and enable (1) #CHIP_CONFIG_SECURITY_MGR_TIME_ALERTS_PLATFORM option.
 *
 */
extern void OnTimeConsumingCryptoStart(void);

/**
 * This function is called to notify the application when a time-consuming
 * cryptographic operation has just finished.
 *
 * @note If application wants to receive these alerts and adjust platform settings
 *       accordingly then it should provide its own implementation of these functions
 *       and enable (1) #CHIP_CONFIG_SECURITY_MGR_TIME_ALERTS_PLATFORM option.
 *
 */
extern void OnTimeConsumingCryptoDone(void);

#endif // CHIP_CONFIG_SECURITY_MGR_TIME_ALERTS_PLATFORM

} // namespace Security
} // namespace Platform

using chip::Profiles::Security::CASE::ChipCASEAuthDelegate;
using chip::Profiles::Security::CASE::ChipCASEEngine;
using chip::Profiles::Security::KeyExport::ChipKeyExport;
using chip::Profiles::Security::KeyExport::ChipKeyExportDelegate;
using chip::Profiles::Security::PASE::ChipPASEEngine;
using chip::Profiles::Security::TAKE::ChipTAKEChallengerAuthDelegate;
using chip::Profiles::Security::TAKE::ChipTAKEEngine;
using chip::Profiles::Security::TAKE::ChipTAKETokenAuthDelegate;
using chip::Profiles::StatusReporting::StatusReport;

class DLL_EXPORT ChipSecurityManager
{
public:
    enum State
    {
        kState_NotInitialized = 0,
        kState_Idle,
        kState_CASEInProgress,
        kState_PASEInProgress,
        kState_TAKEInProgress,
        kState_KeyExportInProgress
    };

    ChipFabricState * FabricState;         // [READ ONLY] Associated Fabric State object.
    ChipExchangeManager * ExchangeManager; // [READ ONLY] Associated Exchange Manager object.
    uint8_t State;                         // [READ ONLY] State of the CHIP Message Layer object
#if CHIP_CONFIG_ENABLE_CASE_INITIATOR
    uint32_t InitiatorCASEConfig;        // CASE configuration proposed when initiating a CASE session
    uint32_t InitiatorCASECurveId;       // ECDH curve proposed when initiating a CASE session
    uint8_t InitiatorAllowedCASEConfigs; // Set of allowed CASE configurations when initiating a CASE session
    uint8_t InitiatorAllowedCASECurves;  // Set of allowed ECDH curves when initiating a CASE session
#endif
#if CHIP_CONFIG_ENABLE_CASE_RESPONDER
    uint8_t ResponderAllowedCASEConfigs; // Set of allowed CASE configurations when responding to CASE session
    uint8_t ResponderAllowedCASECurves;  // Set of allowed ECDH curves when responding to CASE session
#endif
#if CHIP_CONFIG_ENABLE_KEY_EXPORT_INITIATOR
    uint8_t InitiatorKeyExportConfig;         // Key export configuration proposed when initiating key export request
    uint8_t InitiatorAllowedKeyExportConfigs; // Set of allowed configurations when initiating key export request
#endif
#if CHIP_CONFIG_ENABLE_KEY_EXPORT_RESPONDER
    uint8_t ResponderAllowedKeyExportConfigs; // Set of allowed configurations when responding to key export request
#endif
#if CHIP_CONFIG_SECURITY_TEST_MODE
    bool CASEUseKnownECDHKey; // Enable the use of a known ECDH key pair in CASE to allow man-in-the-middle
                              // key recovery for testing purposes.
#endif
    uint32_t SessionEstablishTimeout; // The amount of time after which an in-progress session establishment will timeout.
    uint32_t IdleSessionTimeout;      // The amount of time after which an idle session will be removed.

    ChipSecurityManager(void);

    CHIP_ERROR Init(ChipExchangeManager & aExchangeMgr, System::Layer & aSystemLayer);
    CHIP_ERROR Shutdown(void);

#if CHIP_CONFIG_PROVIDE_OBSOLESCENT_INTERFACES
    CHIP_ERROR Init(ChipExchangeManager * aExchangeMgr, InetLayer * aInetLayer);
#endif // CHIP_CONFIG_PROVIDE_OBSOLESCENT_INTERFACES

    typedef void (*SessionEstablishedFunct)(ChipSecurityManager * sm, ChipConnection * con, void * reqState, uint16_t sessionKeyId,
                                            uint64_t peerNodeId, uint8_t encType);
    typedef void (*SessionErrorFunct)(ChipSecurityManager * sm, ChipConnection * con, void * reqState, CHIP_ERROR localErr,
                                      uint64_t peerNodeId, StatusReport * statusReport);

    /**
     * Type of key error message handling function.
     *
     * @param[in] keyId         Encryption key caused the key error message response from the peer.
     * @param[in] encType       Encryption type associated with @a keyId.
     * @param[in] messageId     The identifier of the CHIP message resulted in the key error response from the peer.
     * @param[in] peerNodeId    The identifier of the CHIP node that sent key error message.
     * @param[in] keyErr        The error code received from the peer.
     *
     */
    typedef void (*KeyErrorMsgRcvdFunct)(uint16_t keyId, uint8_t encType, uint32_t messageId, uint64_t peerNodeId,
                                         CHIP_ERROR keyErr);

    /**
     * Type of key export protocol complete handling function.
     *
     * @param[in] sm             A pointer to ChipSecurityManager object.
     * @param[in] con            A pointer to ChipConnection object.
     * @param[in] reqState       A pointer to the key export requester state.
     * @param[in] keyId          Exported key ID.
     * @param[in] exportedKey    A pointer to the exported secret key.
     * @param[in] exportedKeyLen A reference to the exported secret key length.
     *
     */
    typedef void (*KeyExportCompleteFunct)(ChipSecurityManager * sm, ChipConnection * con, void * reqState, uint32_t exportedKeyId,
                                           const uint8_t * exportedKey, uint16_t exportedKeyLen);

    /**
     * Type of key export protocol error handling function.
     *
     * @param[in] sm             A pointer to ChipSecurityManager object.
     * @param[in] con            A pointer to ChipConnection object.
     * @param[in] reqState       A pointer to the key export requester state.
     * @param[in] localErr       The CHIP_ERROR encountered during key export protocol.
     * @param[in] statusReport   A pointer to StatusReport object if error status received from peer.
     *
     */
    typedef void (*KeyExportErrorFunct)(ChipSecurityManager * sm, ChipConnection * con, void * reqState, CHIP_ERROR localErr,
                                        StatusReport * statusReport);

    // Initiate a secure PASE session, optionally providing a password.
    // Session establishment is done over connection that was specified.
    CHIP_ERROR StartPASESession(ChipConnection * con, ChipAuthMode requestedAuthMode, void * reqState,
                                SessionEstablishedFunct onComplete, SessionErrorFunct onError, const uint8_t * pw = NULL,
                                uint16_t pwLen = 0);

    // Initiate a secure CASE session, optionally providing a CASE auth delegate.
    // Session establishment is done over specified connection or over UDP using RMP Protocol.
    CHIP_ERROR StartCASESession(ChipConnection * con, uint64_t peerNodeId, const IPAddress & peerAddr, uint16_t peerPort,
                                ChipAuthMode requestedAuthMode, void * reqState, SessionEstablishedFunct onComplete,
                                SessionErrorFunct onError, ChipCASEAuthDelegate * authDelegate = NULL,
                                uint64_t terminatingNodeId = kNodeIdNotSpecified);

    // Initiate a secure TAKE session, optionally providing a TAKE auth delegate.
    // Session establishment is done over connection that was specified.
    CHIP_ERROR StartTAKESession(ChipConnection * con, ChipAuthMode requestedAuthMode, void * reqState,
                                SessionEstablishedFunct onComplete, SessionErrorFunct onError, bool encryptAuthPhase,
                                bool encryptCommPhase, bool timeLimitedIK, bool sendChallengerId,
                                ChipTAKEChallengerAuthDelegate * authDelegate = NULL);

    // Initiate key export protocol.
    CHIP_ERROR StartKeyExport(ChipConnection * con, uint64_t peerNodeId, const IPAddress & peerAddr, uint16_t peerPort,
                              uint32_t keyId, bool signMessage, void * reqState, KeyExportCompleteFunct onComplete,
                              KeyExportErrorFunct onError, ChipKeyExportDelegate * keyExportDelegate = NULL);

    // General callback functions. These will be called when a secure session is established or fails.
    SessionEstablishedFunct OnSessionEstablished;
    SessionErrorFunct OnSessionError;

    /**
     * The key error callback function. This function is called when
     * a key error message is received.
     */
    KeyErrorMsgRcvdFunct OnKeyErrorMsgRcvd;

    void SetCASEAuthDelegate(ChipCASEAuthDelegate * delegate)
    {
#if CHIP_CONFIG_ENABLE_CASE_INITIATOR || CHIP_CONFIG_ENABLE_CASE_RESPONDER
        mDefaultAuthDelegate = delegate;
#endif
    }

    void SetTAKEAuthDelegate(ChipTAKEChallengerAuthDelegate * delegate)
    {
#if CHIP_CONFIG_ENABLE_TAKE_INITIATOR
        mDefaultTAKEChallengerAuthDelegate = delegate;
#endif
    }

    void SetTAKETokenAuthDelegate(ChipTAKETokenAuthDelegate * delegate)
    {
#if CHIP_CONFIG_ENABLE_TAKE_RESPONDER
        mDefaultTAKETokenAuthDelegate = delegate;
#endif
    }

    void SetKeyExportDelegate(ChipKeyExportDelegate * delegate)
    {
#if CHIP_CONFIG_ENABLE_KEY_EXPORT_INITIATOR || CHIP_CONFIG_ENABLE_KEY_EXPORT_RESPONDER
        mDefaultKeyExportDelegate = delegate;
#endif
    }

    // Determine whether CHIP error code is a key error.
    bool IsKeyError(CHIP_ERROR err);

    // Send key error message when correct key has not been found and the message cannot be decrypted.
    CHIP_ERROR SendKeyErrorMsg(ChipMessageInfo * rcvdMsgInfo, const IPPacketInfo * rcvdMsgPacketInfo, ChipConnection * con,
                               CHIP_ERROR keyErr);

    void OnEncryptedMsgRcvd(uint16_t sessionKeyId, uint64_t peerNodeId, uint8_t encType);

#if CHIP_CONFIG_USE_APP_GROUP_KEYS_FOR_MSG_ENC
    // Send message counter synchronization message.
    CHIP_ERROR SendMsgCounterSyncResp(const ChipMessageInfo * rcvdMsgInfo, const IPPacketInfo * rcvdMsgPacketInfo);

    // Send peer message counter synchronization request.
    CHIP_ERROR SendSolitaryMsgCounterSyncReq(const ChipMessageInfo * rcvdMsgInfo, const IPPacketInfo * rcvdMsgPacketInfo);

    // Handle message counter synchronization response message.
    void HandleMsgCounterSyncRespMsg(ChipMessageInfo * msgInfo, PacketBuffer * msgBuf);
#endif

    CHIP_ERROR CancelSessionEstablishment(void * reqState);

    void ReserveKey(uint64_t peerNodeId, uint16_t keyId);
    void ReleaseKey(uint64_t peerNodeId, uint16_t keyId);

private:
    enum Flags
    {
        kFlag_IdleSessionTimerRunning = 0x01
    };

    ExchangeContext * mEC;
    ChipConnection * mCon;
    union
    {
#if CHIP_CONFIG_ENABLE_PASE_INITIATOR || CHIP_CONFIG_ENABLE_PASE_RESPONDER
        ChipPASEEngine * mPASEEngine;
#endif
#if CHIP_CONFIG_ENABLE_CASE_INITIATOR || CHIP_CONFIG_ENABLE_CASE_RESPONDER
        ChipCASEEngine * mCASEEngine;
#endif
#if CHIP_CONFIG_ENABLE_TAKE_INITIATOR || CHIP_CONFIG_ENABLE_TAKE_RESPONDER
        ChipTAKEEngine * mTAKEEngine;
#endif
#if CHIP_CONFIG_ENABLE_KEY_EXPORT_INITIATOR
        ChipKeyExport * mKeyExport;
#endif
    };
    union
    {
        SessionEstablishedFunct mStartSecureSession_OnComplete;

        /**
         * The key export protocol complete callback function. This function is
         * called when the secret key export process is complete.
         */
        KeyExportCompleteFunct mStartKeyExport_OnComplete;
    };
    union
    {
        SessionErrorFunct mStartSecureSession_OnError;

        /**
         * The key export protocol error callback function. This function is
         * called when an error is encountered during key export process.
         */
        KeyExportErrorFunct mStartKeyExport_OnError;
    };
    union
    {
        void * mStartSecureSession_ReqState;
        void * mStartKeyExport_ReqState;
    };
#if CHIP_CONFIG_ENABLE_PASE_RESPONDER
    uint32_t mPASERateLimiterTimeout;
    uint8_t mPASERateLimiterCount;
    void UpdatePASERateLimiter(CHIP_ERROR err);
#endif
#if CHIP_CONFIG_ENABLE_CASE_INITIATOR || CHIP_CONFIG_ENABLE_CASE_RESPONDER
    ChipCASEAuthDelegate * mDefaultAuthDelegate;
#endif
#if CHIP_CONFIG_ENABLE_TAKE_INITIATOR
    ChipTAKEChallengerAuthDelegate * mDefaultTAKEChallengerAuthDelegate;
#endif
#if CHIP_CONFIG_ENABLE_TAKE_RESPONDER
    ChipTAKETokenAuthDelegate * mDefaultTAKETokenAuthDelegate;
#endif
#if CHIP_CONFIG_ENABLE_KEY_EXPORT_INITIATOR || CHIP_CONFIG_ENABLE_KEY_EXPORT_RESPONDER
    ChipKeyExportDelegate * mDefaultKeyExportDelegate;
#endif

    uint16_t mSessionKeyId;
    ChipAuthMode mRequestedAuthMode;
    uint8_t mEncType;
    System::Layer * mSystemLayer;
    uint8_t mFlags;

    void StartSessionTimer(void);
    void CancelSessionTimer(void);
    static void HandleSessionTimeout(System::Layer * aSystemLayer, void * aAppState, System::Error aError);

    void StartIdleSessionTimer(void);
    void StopIdleSessionTimer(void);
    static void HandleIdleSessionTimeout(System::Layer * aLayer, void * aAppState, System::Error aError);

    static void HandleUnsolicitedMessage(ExchangeContext * ec, const IPPacketInfo * pktInfo, const ChipMessageInfo * msgInfo,
                                         uint32_t profileId, uint8_t msgType, PacketBuffer * msgBuf);

    void StartPASESession(void);
    void HandlePASESessionStart(ExchangeContext * ec, const IPPacketInfo * pktInfo, const ChipMessageInfo * msgInfo,
                                PacketBuffer * msgBuf);
    CHIP_ERROR ProcessPASEInitiatorStep1(ExchangeContext * ec, PacketBuffer * msgBuf);
    CHIP_ERROR SendPASEResponderReconfigure(void);
    CHIP_ERROR SendPASEResponderStep1(void);
    CHIP_ERROR SendPASEResponderStep2(void);
    CHIP_ERROR SendPASEInitiatorStep1(uint32_t paseConfig);
    CHIP_ERROR ProcessPASEResponderReconfigure(PacketBuffer * msgBuf, uint32_t & newConfig);
    CHIP_ERROR ProcessPASEResponderStep1(PacketBuffer * msgBuf);
    CHIP_ERROR ProcessPASEResponderStep2(PacketBuffer * msgBuf);
    CHIP_ERROR SendPASEInitiatorStep2(void);
    CHIP_ERROR ProcessPASEInitiatorStep2(PacketBuffer * msgBuf);
    CHIP_ERROR SendPASEResponderKeyConfirm(void);
    CHIP_ERROR ProcessPASEResponderKeyConfirm(PacketBuffer * msgBuf);
    static void HandlePASEMessageInitiator(ExchangeContext * ec, const IPPacketInfo * pktInfo, const ChipMessageInfo * msgInfo,
                                           uint32_t profileId, uint8_t msgType, PacketBuffer * msgBuf);
    static void HandlePASEMessageResponder(ExchangeContext * ec, const IPPacketInfo * pktInfo, const ChipMessageInfo * msgInfo,
                                           uint32_t profileId, uint8_t msgType, PacketBuffer * msgBuf);

    void StartCASESession(uint32_t config, uint32_t curveId);
    void HandleCASESessionStart(ExchangeContext * ec, const IPPacketInfo * pktInfo, const ChipMessageInfo * msgInfo,
                                PacketBuffer * msgBuf);
    static void HandleCASEMessageInitiator(ExchangeContext * ec, const IPPacketInfo * pktInfo, const ChipMessageInfo * msgInfo,
                                           uint32_t profileId, uint8_t msgType, PacketBuffer * msgBuf);
    static void HandleCASEMessageResponder(ExchangeContext * ec, const IPPacketInfo * pktInfo, const ChipMessageInfo * msgInfo,
                                           uint32_t profileId, uint8_t msgType, PacketBuffer * msgBuf);

    void StartTAKESession(bool encryptAuthPhase, bool encryptCommPhase, bool timeLimitedIK, bool sendChallengerId);
    void HandleTAKESessionStart(ExchangeContext * ec, const IPPacketInfo * pktInfo, const ChipMessageInfo * msgInfo,
                                PacketBuffer * msgBuf);
    CHIP_ERROR SendTAKEIdentifyToken(uint8_t takeConfig, bool encryptAuthPhase, bool encryptCommPhase, bool timeLimitedIK,
                                     bool sendChallengerId);
    static void HandleTAKEMessageInitiator(ExchangeContext * ec, const IPPacketInfo * pktInfo, const ChipMessageInfo * msgInfo,
                                           uint32_t profileId, uint8_t msgType, PacketBuffer * msgBuf);
    static void HandleTAKEMessageResponder(ExchangeContext * ec, const IPPacketInfo * pktInfo, const ChipMessageInfo * msgInfo,
                                           uint32_t profileId, uint8_t msgType, PacketBuffer * msgBuf);
    CHIP_ERROR ProcessTAKEIdentifyTokenResponse(const PacketBuffer * msgBuf);
    CHIP_ERROR CreateTAKESecureSession(void);
    CHIP_ERROR SendTAKEAuthenticateToken(void);
    CHIP_ERROR ProcessTAKEAuthenticateToken(const PacketBuffer * msgBuf);
    CHIP_ERROR SendTAKEAuthenticateTokenResponse(void);
    CHIP_ERROR ProcessTAKEAuthenticateTokenResponse(const PacketBuffer * msgBuf);
    CHIP_ERROR SendTAKEReAuthenticateToken(void);
    CHIP_ERROR ProcessTAKEReAuthenticateToken(const PacketBuffer * msgBuf);
    CHIP_ERROR SendTAKEReAuthenticateTokenResponse(void);
    CHIP_ERROR ProcessTAKEReAuthenticateTokenResponse(const PacketBuffer * msgBuf);
    CHIP_ERROR SendTAKETokenReconfigure(void);
    CHIP_ERROR ProcessTAKETokenReconfigure(uint8_t & config, const PacketBuffer * msgBuf);
    CHIP_ERROR FinishTAKESetUp(void);

    void HandleKeyErrorMsg(ExchangeContext * ec, PacketBuffer * msgBuf);

#if CHIP_CONFIG_USE_APP_GROUP_KEYS_FOR_MSG_ENC
    CHIP_ERROR NewMsgCounterSyncExchange(const ChipMessageInfo * rcvdMsgInfo, const IPPacketInfo * rcvdMsgPacketInfo,
                                         ExchangeContext *& ec);
#endif
    CHIP_ERROR NewSessionExchange(uint64_t peerNodeId, IPAddress peerAddr, uint16_t peerPort);
    CHIP_ERROR HandleSessionEstablished(void);
    void HandleSessionComplete(void);
    void HandleSessionError(CHIP_ERROR err, PacketBuffer * statusReportMsgBuf);
    static void HandleConnectionClosed(ExchangeContext * ec, ChipConnection * con, CHIP_ERROR conErr);

    static CHIP_ERROR SendStatusReport(CHIP_ERROR localError, ExchangeContext * ec);

    void HandleKeyExportRequest(ExchangeContext * ec, const IPPacketInfo * pktInfo, const ChipMessageInfo * msgInfo,
                                PacketBuffer * msgBuf);
    CHIP_ERROR SendKeyExportRequest(uint8_t keyExportConfig, uint32_t keyId, bool signMessage);
    CHIP_ERROR SendKeyExportResponse(ChipKeyExport & keyExport, uint8_t msgType, const ChipMessageInfo * msgInfo);
    static void HandleKeyExportMessageInitiator(ExchangeContext * ec, const IPPacketInfo * pktInfo, const ChipMessageInfo * msgInfo,
                                                uint32_t profileId, uint8_t msgType, PacketBuffer * msgBuf);
    void HandleKeyExportError(CHIP_ERROR err, PacketBuffer * statusReportMsgBuf);

#if CHIP_CONFIG_ENABLE_RELIABLE_MESSAGING
    static void RMPHandleAckRcvd(ExchangeContext * ec, void * msgCtxt);
    static void RMPHandleSendError(ExchangeContext * ec, CHIP_ERROR err, void * msgCtxt);
#endif // CHIP_CONFIG_ENABLE_RELIABLE_MESSAGING

    void Reset(void);

    void AsyncNotifySecurityManagerAvailable();
    static void DoNotifySecurityManagerAvailable(System::Layer * systemLayer, void * appState, System::Error err);

    void ReserveSessionKey(ChipSessionKey * sessionKey);
    void ReleaseSessionKey(ChipSessionKey * sessionKey);
};

} // namespace chip

#endif /* CHIPSECURITYMANAGER_H_ */
