Texture Class
=============

Definition
----------

.. class:: _Texture : public _Asset

.. type:: Ref<_Texture> Texture
    Ref_w<_Texture> pTexture

    A texture object that contains a 2D image.

Constructors
------------

.. function:: Texture Texture::New(uint w, uint h, bool hdr, const TextureOptions& opts)
    Texture Texture::New(uint w, uint h, GLenum type, const TextureOptions& opts)

    Creates an empty texture with the given parameters.

.. function:: Texture Texture::New(const std::string& path, const TextureOptions& opts = TextureOptions(), bool async = false)

    Loads the texture image from :expr:`path` with the given options.
    If :expr:`async` is true, the image may load in the background.

Variable functions
------------------

.. function:: const uint& _Texture::width() const

    the width of the loaded texture, or 0 if empty

Method functions
----------------

.. function:: void _Texture::Bind()

    Binds the texture for rendering.
    If the texture is loading, the function will wait for it to finish.

.. function:: void _Texture::Unbind() const

    Unbinds the texture.

.. function:: void _Texture::Blit(const RenderTarget& dst, const Material& mat)

    Copies the texture to target :expr:`dst` through material :expr:`mat`.
    If the material is null, the texture is copied directly.


Texture Options Struct
======================

Definition
----------

.. struct:: TextureOptions

Variables
---------

.. var:: TextureWrap xwrap

.. var:: TextureWrap ywrap

.. var:: int mipmaps

.. var:: bool linear

Texture Wrap Enum
=================

Definition
----------

.. enum-class:: TextureWrap

.. enumerator:: TextureWrap::Clamp

.. enumerator:: TextureWrap::Repeat

.. enumerator:: TextureWrap::Mirror