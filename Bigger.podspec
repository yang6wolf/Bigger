Pod::Spec.new do |s|

  s.name         = "Bigger"
  s.version      = "0.0.1"
  s.summary      = "贵金属项目所使用的第三方组件(非UI部分)"
  s.homepage     = "https://git.ms.netease.com/preciousmetals/Bigger"
  s.license      = { :type => 'MIT', :file => 'LICENSE'}
  s.author       = { "YangXP" => "bjyangxiaopeng1@corp.netease.com" }
  s.source       = { :git => "https://git.ms.netease.com/preciousmetals/Bigger.git", :tag => s.version.to_s}

  s.source_files = 'Bigger/*.{h,cc}'
 # s.private_header_files = 'Bigger/BLogger.h'
  s.public_header_files = 'Bigger/Bigger.h', 'Bigger/BLoggerInterface.h'
  s.platform = :ios
  s.ios.deployment_target = '8.0'

end
