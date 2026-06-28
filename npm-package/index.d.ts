declare module '@primordialomegazero/pozd-fhe-client' {
  interface ClientKeys {
    seed: number;
    client_id: string;
  }

  interface PublicInfo {
    client_id: string;
    version: string;
    protocol: string;
    edition: string;
  }

  class PozDFClient {
    phi: number;
    lambda: number;
    clientId: string;
    seed: number;

    constructor(seed?: number | null);
    encrypt(message: number): number;
    decrypt(encryptedValue: number): number;
    getRegistrationPayload(): object;
    getAddPayload(e1: number, e2: number): object;
    getMultiplyPayload(e1: number, e2: number): object;
    getAntiMatterPayload(): object;
    getPQCPayload(): object;
    getZKPPayload(secret: number, challenge: number): object;
    getSupplyChainPayload(): object;
    getTPSPayload(): object;
    getHealthPayload(): object;
    getPublicInfo(): PublicInfo;
    getSecretKeys(): ClientKeys;
    static fromKeys(keys: ClientKeys): PozDFClient;
  }

  export { PozDFClient, ClientKeys, PublicInfo };
}
