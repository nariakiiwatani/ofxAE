# ofxAE
by nariakiiwatani

## 20140427ワークショップ用ブランチ
[当日ドキュメント](http://piratepad.net/ofxAEWS20140427 "piratepad")

## 概要
Adobe AfterEffects(AE)のコンポジションをopenFrameworksのリアルタイムアニメーションにエクスポートするofxaddonを作るプロジェクトです。  
未完成なので、pull requestやissueをお待ちしてます！

## 必要addon、ライブラリ等
- [ofxMask](https://github.com/nariakiiwatani/ofxMask "ofxMask")
- [ofxJSON](https://github.com/jefftimesten/ofxJSON "ofxJSON")

## 使用法
### AEから書き出し
1. プロジェクトペインで書き出したいコンポジションを選択
2. 「ファイル」-->スクリプト-->実行」でtool/compExport.jsxを選択し、データを保存するフォルダを選択

### OFで再生
ofApp.h  

	ofxAE::Loader loader_;
	ofxAE::Composition *composition_;
ofApp.cpp  

	void ofApp::setup(){
		ofxAE::Loader loader("exported_folder");
		composition_ = loader_.loadComposition("foo.json");	// "bin/data/exported_folder/foo.json"
		composition_->setLoopState(FrameCounter::LOOP_ONEWAY);
	}

	//--------------------------------------------------------------
	void ofApp::update(){
		composition_->update();
	}

	//--------------------------------------------------------------
	void ofApp::draw(){
		ofBackground(0);
		composition_->draw();
	}

## 目標
1. AEの全機能をOFでサポート  
とはいえAEはOFと違ってリアルタイムが前提でないので、マシンパワー的にも思想的にも完全なエンジンは作れないと思われるので  
スクリプトからの書き出し時またはOFでのロード時に機能毎にenable/disableスイッチを設ける方向を考えています。  

2. アニメーションをインタラクティブに操作するインターフェースを提供  
このaddonを作ろうと思った動機で、これが最重要です。  
画像の差し替え、インタラクティブなロケーター制御、リアルタイムエフェクトなどなど・・・  

3. AE以外のアニメーション作成ソフトをサポート  
ofxAEと名づけてはいますが、同じフォーマットのjsonが書き出せれば他のソフトから書きだした情報でも当然使用できます。  
当面AEにフォーカスして進めますが、AEとOFの独立性は保ちながらやっていきたいです。  
