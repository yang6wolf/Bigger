#
# Be sure to run `pod lib lint Bigger.podspec' to ensure this is a
# valid spec before submitting.
#
# Any lines starting with a # are optional, but their use is encouraged
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html
#

Pod::Spec.new do |s|

    s.name         = "Bigger"
    s.version      = "0.1.0"
    s.summary      = "贵金属项目所使用的第三方组件(非UI部分)"
    s.homepage     = "https://git.ms.netease.com/preciousmetals/Bigger"
    s.license      = { :type => 'MIT', :file => 'LICENSE'}
    s.author       = { "YangXP" => "bjyangxiaopeng1@corp.netease.com" }
    s.source       = { :git => "https://git.ms.netease.com/preciousmetals/Bigger.git", :tag => s.version.to_s}

    s.source_files = 'Bigger/**/*.{h,cc,cpp}'
    # s.private_header_files = 'Bigger/Classes/BLogger.h'
<<<<<<< HEAD
    s.public_header_files = 'Bigger/include/*.h'
=======
    s.public_header_files = 'Bigger/Classes/BCommonDef.h', 'Bigger/Classes/Bigger.h', 'Bigger/Classes/BLoggerWrapper.h', 'Bigger/Classes/BAgent.h'
>>>>>>> 给filewrite的开启接口添加是否加密参数，对Example工程补充了Demo View。
    s.platform = :ios
    s.ios.deployment_target = '8.0'
    s.dependency 'LDXLog'

end
