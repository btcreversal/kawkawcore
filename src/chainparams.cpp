// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Copyright (c) 2017-2020 The OLDNAMENEEDKEEP__Core developers
// Copyright (c) 2022 The Kawkaw core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"
#include "arith_uint256.h"

#include <assert.h>
#include "chainparamsseeds.h"

//TODO: Take these out
extern double algoHashTotal[16];
extern int algoHashHits[16];


static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << CScriptNum(0) << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "A good cook shouldent look in a cochery book";
    const CScript genesisOutputScript = CScript() << ParseHex("00099992f5b2e2f02065c8a1706525fe95e249b24a594469a3c4cf7a51b8b2cc227fe6f391867a596d32e8057e5a6bc0f4a6b71f71851dc3278eecc70ec2ad4268") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

void CChainParams::TurnOffSegwit() {
	consensus.nSegwitEnabled = false;
}

void CChainParams::TurnOffCSV() {
	consensus.nCSVEnabled = false;
}

void CChainParams::TurnOffBIP34() {
	consensus.nBIP34Enabled = false;
}

void CChainParams::TurnOffBIP65() {
	consensus.nBIP65Enabled = false;
}

void CChainParams::TurnOffBIP66() {
	consensus.nBIP66Enabled = false;
}

bool CChainParams::BIP34() {
	return consensus.nBIP34Enabled;
}

bool CChainParams::BIP65() {
	return consensus.nBIP34Enabled;
}

bool CChainParams::BIP66() {
	return consensus.nBIP34Enabled;
}

bool CChainParams::CSVEnabled() const{
	return consensus.nCSVEnabled;
}


/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 2100000;  //~ 4 yrs at 1 min block time
        consensus.nBIP34Enabled = true;
        consensus.nBIP65Enabled = true; // 
        consensus.nBIP66Enabled = true;
        consensus.nSegwitEnabled = true;
        consensus.nCSVEnabled 	= true;
        consensus.powLimit 	= uint256S("00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.kawpowLimit 	= uint256S("00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); 
        consensus.nPowTargetTimespan = 2016 * 60; // 1.4 days
        consensus.nPowTargetSpacing = 1 * 60;
	consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1613; // Approx 80% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1657864772; // Friday, 20 May 2022 00:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1689337592; // Monday, 23 May 2022 00:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideRuleChangeActivationThreshold = 1814;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideMinerConfirmationWindow = 2016;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].bit = 6;  //Assets (HIP2)
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nStartTime = 1657864772; // Friday, 20 May 2022 00:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nTimeout = 1689337592; // Monday, 23 May 2022 00:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideRuleChangeActivationThreshold = 1814;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideMinerConfirmationWindow = 2016;
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].bit = 7;  // Assets (HIP5)
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nStartTime = 1657864772; // Friday, 20 May 2022 00:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nTimeout = 1689337592; // Monday, 23 May 2022 00:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideRuleChangeActivationThreshold = 1714; // Approx 85% of 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideMinerConfirmationWindow = 2016;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].bit = 8;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nStartTime = 1657864772; // Friday, 20 May 2022 00:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nTimeout = 1689337592; // Monday, 23 May 2022 00:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideRuleChangeActivationThreshold = 1714; // Approx 85% of 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideMinerConfirmationWindow = 2016;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].bit = 9;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nStartTime = 1657864772; // Friday, 20 May 2022 00:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nTimeout = 1689337592; // Monday, 23 May 2022 00:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideRuleChangeActivationThreshold = 1411; // Approx 70% of 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideMinerConfirmationWindow = 2016;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].bit = 10;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nStartTime = 1657864772; // Friday, 20 May 2022 00:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nTimeout = 1689337592; // Monday, 23 May 2022 00:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideRuleChangeActivationThreshold = 1411; // Approx 70% of 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideMinerConfirmationWindow = 2016; 
	
	consensus.BIP34LockedIn = 6048; // Locked_in at height 6048


        // The best chain should have at least this much work
        //consensus.nMinimumChainWork = uint256S("0x"); // 

        // By default assume that the signatures in ancestors of this block are valid. Block# 
        //consensus.defaultAssumeValid = uint256S("0x"); // block 0

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0x4B; // K
        pchMessageStart[1] = 0x41; // A
        pchMessageStart[2] = 0x57; // W
        pchMessageStart[3] = 0x4B; // K
        nDefaultPort = 9090;
        nPruneAfterHeight = 100000;		
		
		uint32_t nGenesisTime = 1657864772;	
		genesis = CreateGenesisBlock(nGenesisTime, 10396369, 0x1e00ffff, 4, 5000 * COIN);
        consensus.hashGenesisBlock = genesis.GetX16RHash();	
	    assert(consensus.hashGenesisBlock == uint256S("000000315cc6f03c1448d6ed3e049424e55af4b803c422e835cdcd7a89043696"));
        assert(genesis.hashMerkleRoot == uint256S("728fc9fe582c8b27771d5ecf33b4f9f059b7570dde48ce46fbb9ebcef1489bdb"));



	
	 

    vSeeds.emplace_back("161.97.154.202", false);
    vSeeds.emplace_back("seed.kawkawcoin.org", false);
	vSeeds.emplace_back("seed1.kawkawcoin.org", false);
	vSeeds.emplace_back("seed2.kawkawcoin.org", false); 
	vSeeds.emplace_back("seed3.kawkawcoin.org", false); 
	vSeeds.emplace_back("seed4.kawkawcoin.org", false); 
	vSeeds.emplace_back("seed5.kawkawcoin.org", false); 
	vSeeds.emplace_back("seed6.kawkawcoin.org", false); 
	vSeeds.emplace_back("seed7.kawkawcoin.org", false); 

        // Address start with K
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,45);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,122);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,112);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        // KAWKAW BIP44 cointype in mainnet is '968'
        nExtCoinType = 968;

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fMiningRequiresPeers = true;

		checkpointData = (CCheckpointData) {
            {
            	{
            	{0, uint256S("000000315cc6f03c1448d6ed3e049424e55af4b803c422e835cdcd7a89043696")}
		{7604, uint256S("0000000000bea5009ce8aabd54f2df9a6715d2c385686c0ad52b75ab94cb8f57")}
               
}            	
            }
        };

        chainTxData = ChainTxData{
            // Update as we know more about the contents of the Kawkaw chain
        	0, // * UNIX timestamp of last known number of transactions 2021-06-18 22:03:06 UTC
            0,    // * total number of transactions between genesis and that timestamp
            0           //   (the tx=... number in the SetBestChain debug.log lines)
                   // * estimated number of transactions per second after that timestamp
        };


        // Burn Amounts
        nIssueAssetBurnAmount = 1500 * COIN;
        nReissueAssetBurnAmount = 1000* COIN;
        nIssueSubAssetBurnAmount = 1000 * COIN;
        nIssueUniqueAssetBurnAmount = 50 * COIN;
        nIssueMsgChannelAssetBurnAmount = 1000 * COIN;
        nIssueQualifierAssetBurnAmount = 5000 * COIN;
        nIssueSubQualifierAssetBurnAmount = 1000 * COIN;
        nIssueRestrictedAssetBurnAmount = 3500 * COIN;
        nAddNullQualifierTagBurnAmount = .5 * COIN;
		
	    // 5% of each block to team
	    nCommunityAutonomousAmount = 8;

        // Donations Addresses
	    strIssueAssetBurnAddress = "KBew7pTrSxnSuYJYoBcmcJBoFW6aDiiray";
        strReissueAssetBurnAddress = "KBew7pTrSxnSuYJYoBcmcJBoFW6aDiiray";
        strIssueSubAssetBurnAddress = "KBew7pTrSxnSuYJYoBcmcJBoFW6aDiiray";
        strIssueUniqueAssetBurnAddress = "KBew7pTrSxnSuYJYoBcmcJBoFW6aDiiray";
        strIssueMsgChannelAssetBurnAddress = "KBew7pTrSxnSuYJYoBcmcJBoFW6aDiiray";
        strIssueQualifierAssetBurnAddress = "KBew7pTrSxnSuYJYoBcmcJBoFW6aDiiray";
        strIssueSubQualifierAssetBurnAddress = "KBew7pTrSxnSuYJYoBcmcJBoFW6aDiiray";
        strIssueRestrictedAssetBurnAddress = "KBew7pTrSxnSuYJYoBcmcJBoFW6aDiiray";
        strAddNullQualifierTagBurnAddress = "KBew7pTrSxnSuYJYoBcmcJBoFW6aDiiray";
        //Global Burn Address
        strGlobalBurnAddress = "KPGenericBurnXXXXXXXXXXXXXXXYDgYFJ";
        // Proof-Of-Play Address
        strCommunityAutonomousAddress = "KBiRwqpTDH5Qem2LGUUAxZcPUaQW3VpMAH";

        // DGW Activation
        nDGWActivationBlock = 100;

        nMaxReorganizationDepth = 60; // 60 at 1 minute block timespan is +/- 60 minutes.
        nMinReorganizationPeers = 4;
        nMinReorganizationAge = 60 * 60 * 12; // 12 hours

        nAssetActivationHeight = 1; // Asset activated block height
        nMessagingActivationBlock = 1; // Messaging activated block height
        nRestrictedActivationBlock = 1; // Restricted activated block height

        nKAAAWWWPOWActivationTime = 1657867779; // 2021-05-03 06:00:18
        nKAWPOWActivationTime = nKAAAWWWPOWActivationTime;
    }
};

/**
 * Testnet (v7)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 2100000;  //~ 4 yrs at 1 min block time
        consensus.nBIP34Enabled = true;
        consensus.nBIP65Enabled = true; // 
        consensus.nBIP66Enabled = true;
        consensus.nSegwitEnabled = true;
        consensus.nCSVEnabled 	= true;
        consensus.powLimit 	= uint256S("00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.kawpowLimit 	= uint256S("00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // Estimated starting diff for first 180 kawpow blocks
        consensus.nPowTargetTimespan = 2016 * 60; // 1.4 days
        consensus.nPowTargetSpacing = 1 * 60;
	consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1613; // Approx 80% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1653248479; // Sunday, 22 May 2022 19:41:19
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1653594079; // Thursday, 26 May 2022 19:41:19
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideRuleChangeActivationThreshold = 1814;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideMinerConfirmationWindow = 2016;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].bit = 6;  //Assets (HIP2)
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nStartTime = 1653248479; // Sunday, 22 May 2022 19:41:19
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nTimeout = 1653594079; // Thursday, 26 May 2022 19:41:19
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideRuleChangeActivationThreshold = 1814;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideMinerConfirmationWindow = 2016;
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].bit = 7;  // Assets (HIP5)
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nStartTime = 1653248479; // Sunday, 22 May 2022 19:41:19
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nTimeout = 1653594079; // Thursday, 26 May 2022 19:41:19
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideRuleChangeActivationThreshold = 1714; // Approx 85% of 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideMinerConfirmationWindow = 2016;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].bit = 8;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nStartTime = 1653248479; // Sunday, 22 May 2022 19:41:19
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nTimeout = 1653594079; // Thursday, 26 May 2022 19:41:19
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideRuleChangeActivationThreshold = 1714; // Approx 85% of 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideMinerConfirmationWindow = 2016;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].bit = 9;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nStartTime = 1653248479; // Sunday, 22 May 2022 19:41:19
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nTimeout = 1653594079; // Thursday, 26 May 2022 19:41:19
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideRuleChangeActivationThreshold = 1411; // Approx 70% of 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideMinerConfirmationWindow = 2016;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].bit = 10;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nStartTime = 1653248479; // Sunday, 22 May 2022 19:41:19
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nTimeout = 1653594079; // Thursday, 26 May 2022 19:41:19
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideRuleChangeActivationThreshold = 1411; // Approx 70% of 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideMinerConfirmationWindow = 2016;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");


        pchMessageStart[0] = 0x60;
        pchMessageStart[1] = 0x63;
        pchMessageStart[2] = 0x56;
        pchMessageStart[3] = 0x65;
        nDefaultPort = 4568;
        nPruneAfterHeight = 1000;
		
        uint32_t nGenesisTime = 1653247605;  // Sunday, 22 May 2022 19:26:45

        genesis = CreateGenesisBlock(nGenesisTime, 11903232, 0x1e00ffff, 4, 5000 * COIN);
        consensus.hashGenesisBlock = genesis.GetX16RHash();
	    
       // assert(consensus.hashGenesisBlock == uint256S("00000065c2d5777fe4f059f9ac7579b35c1ad4c7042aebae8c105179cca0f8f0"));
      //  assert(genesis.hashMerkleRoot == uint256S("7c1d71731b98c560a80cee3b88993c8c863342b9661894304fd843bf7e75a41f"));	
		
        vFixedSeeds.clear();
        vSeeds.clear();
	vSeeds.emplace_back("testnet.kawkawcoin.net", false);
		
	base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,68);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,124);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,114);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        // Kawkaw BIP44 cointype in testnet
        nExtCoinType = 1;

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fMiningRequiresPeers = true;

        checkpointData = (CCheckpointData) {
            {
            }
        };

        chainTxData = ChainTxData{
            // Update as we know more about the contents of the Kawkaw chain
            // Stats as of 00000023b66f46d74890287a7b1157dd780c7c5fdda2b561eb96684d2b39d62e window size 43200
            1653247605, // * UNIX timestamp of last known number of transactions
            0,     // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            0.02        // * estimated number of transactions per second after that timestamp
        };

        /** KAWKAW Start **/
        // Burn Amounts
        nIssueAssetBurnAmount = 500 * COIN;
        nReissueAssetBurnAmount = 100 * COIN;
        nIssueSubAssetBurnAmount = 100 * COIN;
        nIssueUniqueAssetBurnAmount = 5 * COIN;
        nIssueMsgChannelAssetBurnAmount = 100 * COIN;
        nIssueQualifierAssetBurnAmount = 1000 * COIN;
        nIssueSubQualifierAssetBurnAmount = 100 * COIN;
        nIssueRestrictedAssetBurnAmount = 1500 * COIN;
        nAddNullQualifierTagBurnAmount = .1 * COIN;
		
	//10% of 5000 COIN to ASSIGN
	nCommunityAutonomousAmount = 15;

        // Burn Addresses
	strIssueAssetBurnAddress = "J1VQJKLSLVZ4syiCAx5hEPq8BrkFaxAXAi";
        strReissueAssetBurnAddress = "J2yh4DiLETuVVDvpvBNSq3QCmHcdMmNEdp";
        strIssueSubAssetBurnAddress = "J3PE3FsHqfszvz7nhwK2Gc32wykrc7pNMA";
        strIssueUniqueAssetBurnAddress = "J4yKRTYF2nRryYEnupsNnQQmRKsQhdspYB";
        strIssueMsgChannelAssetBurnAddress = "J58ndjHjLYKHMszr4ehUg9YMWPAiXNEepa";
        strIssueQualifierAssetBurnAddress = "J68wpmVvdE6bMSkiCEDQWCHCKZs4VVdE2G";
        strIssueSubQualifierAssetBurnAddress = "J7MSidYgNJrPE15ouEsXPYXFYH2AAPXmhr";
        strIssueRestrictedAssetBurnAddress = "J8uX8jfZn14P1VNzh6YjSzLaRTQAdoFSHn";
        strAddNullQualifierTagBurnAddress = "J9CrKy8m548AvSbcv1mcn7tyJQkgcwVfj6";		
	//Global Burn Address
        strGlobalBurnAddress = "JGYQBki6wWWnJLp2dcgdtNZWs9a2e1nXM3";
		
	//CommunityAutonomousAddress
        strCommunityAutonomousAddress = "J8db9nuaVL3Jo8hDcfKh77pZnG2J8jvxWH";

        // DGW Activation
        nDGWActivationBlock = 1;

        nMaxReorganizationDepth = 60; // 60 at 1 minute block timespan is +/- 60 minutes.
        nMinReorganizationPeers = 4;
        nMinReorganizationAge = 60 * 60 * 12; // 12 hours

        nAssetActivationHeight = 1; // Asset activated block height
        nMessagingActivationBlock = 1; // Messaging activated block height
        nRestrictedActivationBlock = 1; // Restricted activated block height

        nKAAAWWWPOWActivationTime = 1653247613; // 2021-05-03 06:00:18
        nKAWPOWActivationTime = nKAAAWWWPOWActivationTime;
        /** KAWKAW End **/
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nBIP34Enabled = true;
        consensus.nBIP65Enabled = true; // 000000000000000004c2b624ed5d7756c508d90fd0da2c7c679febfa6c4735f0
        consensus.nBIP66Enabled = true;
        consensus.nSegwitEnabled = true;
        consensus.nCSVEnabled = true;
        consensus.nSubsidyHalvingInterval = 150;
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.kawpowLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 2016 * 60; // 1.4 days
        consensus.nPowTargetSpacing = 1 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideRuleChangeActivationThreshold = 108;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideMinerConfirmationWindow = 144;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].bit = 6;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideRuleChangeActivationThreshold = 108;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideMinerConfirmationWindow = 144;
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].bit = 7;  // Assets (HIP5)
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nStartTime = 0; // GMT: Sun Mar 3, 2019 5:00:00 PM
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nTimeout = 999999999999ULL; // UTC: Wed Dec 25 2019 07:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideRuleChangeActivationThreshold = 108;
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideMinerConfirmationWindow = 144;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].bit = 8;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideRuleChangeActivationThreshold = 208;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideMinerConfirmationWindow = 288;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].bit = 9;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideRuleChangeActivationThreshold = 108;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideMinerConfirmationWindow = 144;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].bit = 10;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideRuleChangeActivationThreshold = 400;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideMinerConfirmationWindow = 500;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0x44;
        pchMessageStart[1] = 0x52; 
        pchMessageStart[2] = 0x4F; 
        pchMessageStart[3] = 0x57; 
        nDefaultPort = 19444;
        nPruneAfterHeight = 1000;
	    

        genesis = CreateGenesisBlock(1524179366, 1, 0x207fffff, 4, 5000 * COIN);
        consensus.hashGenesisBlock = genesis.GetX16RHash();

        //assert(consensus.hashGenesisBlock == uint256S("0x0b2c703dc93bb63a36c4e33b85be4855ddbca2ac951a7a0a29b8de0408200a3c "));
        //assert(genesis.hashMerkleRoot == uint256S("0x28ff00a867739a352523808d301f504bc4547699398d70faf2266a8bae5f3516"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = (CCheckpointData) {
            {
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };
		
		base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,42);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,124);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,114);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        // Kawkaw BIP44 cointype in regtest
        nExtCoinType = 1;

        /** KAWKAW Start **/
        // Burn Amounts
        nIssueAssetBurnAmount = 500 * COIN;
        nReissueAssetBurnAmount = 100 * COIN;
        nIssueSubAssetBurnAmount = 100 * COIN;
        nIssueUniqueAssetBurnAmount = 5 * COIN;
        nIssueMsgChannelAssetBurnAmount = 100 * COIN;
        nIssueQualifierAssetBurnAmount = 1000 * COIN;
        nIssueSubQualifierAssetBurnAmount = 100 * COIN;
        nIssueRestrictedAssetBurnAmount = 1500 * COIN;
        nAddNullQualifierTagBurnAmount = .1 * COIN;
		
	//10% of 5000 COIN to ASSIGN
	nCommunityAutonomousAmount = 10;

        // Burn Addresses
	strIssueAssetBurnAddress = "J1VQJKLSLVZ4syiCAx5hEPq8BrkFaxAXAi";
        strReissueAssetBurnAddress = "J2yh4DiLETuVVDvpvBNSq3QCmHcdMmNEdp";
        strIssueSubAssetBurnAddress = "J3PE3FsHqfszvz7nhwK2Gc32wykrc7pNMA";
        strIssueUniqueAssetBurnAddress = "J4yKRTYF2nRryYEnupsNnQQmRKsQhdspYB";
        strIssueMsgChannelAssetBurnAddress = "J58ndjHjLYKHMszr4ehUg9YMWPAiXNEepa";
        strIssueQualifierAssetBurnAddress = "J68wpmVvdE6bMSkiCEDQWCHCKZs4VVdE2G";
        strIssueSubQualifierAssetBurnAddress = "J7MSidYgNJrPE15ouEsXPYXFYH2AAPXmhr";
        strIssueRestrictedAssetBurnAddress = "J8uX8jfZn14P1VNzh6YjSzLaRTQAdoFSHn";
        strAddNullQualifierTagBurnAddress = "J9CrKy8m548AvSbcv1mcn7tyJQkgcwVfj6";		
	//Global Burn Address
        strGlobalBurnAddress = "JGYQBki6wWWnJLp2dcgdtNZWs9a2e1nXM3";
		
	//CommunityAutonomousAddress
        strCommunityAutonomousAddress = "JCPncGFawSDgP3CmG19MB6cbKP5XuhXY4u";

        // DGW Activation
        nDGWActivationBlock = 200;

        nMaxReorganizationDepth = 60; // 60 at 1 minute block timespan is +/- 60 minutes.
        nMinReorganizationPeers = 4;
        nMinReorganizationAge = 60 * 60 * 12; // 12 hours

        nAssetActivationHeight = 0; // Asset activated block height
        nMessagingActivationBlock = 0; // Messaging activated block height
        nRestrictedActivationBlock = 0; // Restricted activated block height

        // TODO, we need to figure out what to do with this for regtest. This effects the unit tests
        // For now we can use a timestamp very far away
        // If you are looking to test the kawpow hashing function in regtest. You will need to change this number
        nKAAAWWWPOWActivationTime = 3582830167;
        nKAWPOWActivationTime = nKAAAWWWPOWActivationTime;
        /** KAWKAW End **/
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &GetParams() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network, bool fForceBlockNetwork)
{
    SelectBaseParams(network);
    if (fForceBlockNetwork) {
        bNetwork.SetNetwork(network);
    }
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}

void TurnOffSegwit(){
	globalChainParams->TurnOffSegwit();
}

void TurnOffCSV() {
	globalChainParams->TurnOffCSV();
}

void TurnOffBIP34() {
	globalChainParams->TurnOffBIP34();
}

void TurnOffBIP65() {
	globalChainParams->TurnOffBIP65();
}

void TurnOffBIP66() {
	globalChainParams->TurnOffBIP66();
}
