#include "PostEffect.h"
#include <d3dx12.h>

PostEffect* PostEffect::Create(UINT texNumber, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
{
	// 3Dオブジェクトのインスタンスを生成
	PostEffect* instance = new PostEffect();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize(texNumber, anchorpoint, isFlipX, isFlipY)) {
		delete instance;
		assert(0);
		return nullptr;
	}
	instance->SetPosition({ 10, 10 });
	instance->SetSize({ 100, 100 });
	instance->Update();

	return instance;
}

void PostEffect::Draw()
{
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	ID3D12GraphicsCommandList* cmdList = spriteCommon->GetCmdList();
	PipelineSet pipelineSet = spriteCommon->GetPipelineSet();

	//パイプラインステートの設定
	cmdList->SetPipelineState(pipelineSet.pipelinestate.Get());
	//ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(pipelineSet.rootsignature.Get());
	//プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//テクスチャ用デスクリプタヒープの設定
	ID3D12DescriptorHeap* ppHeap[] = { spriteCommon->GetDescHeap().Get()};
	cmdList->SetDescriptorHeaps(_countof(ppHeap), ppHeap);

	//頂点バッファをセット
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	//ルートパラメータ0番に定数バッファをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	//ルートパラメータ1番にシェーダリソースビューをセット
	spriteCommon->SetGraphicsRootDescriptorTable(1, texNumber);

	//ポリゴンの描画(4頂点で四角形)
	cmdList->DrawInstanced(4, 1, 0, 0);
}
